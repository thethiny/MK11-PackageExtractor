//#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <direct.h>
#include <windows.h>

#include "mk11file.h"
#include "package.h"
#include "subpackage.h"
#include "segment_info.h"
#include "compressedsegment.h"
#include "compression.h"
#include "oodle.h"
#include "zlib.h"

using namespace std;

int main(int argv, const char* argc[])
{
    MK11File mk11_obj = MK11File();
    ArgumentParser arg_parse;
    try
    {
        arg_parse.set(argv, argc);
    }
    catch (string s)
    {
        cerr<<s<<endl;
        return 1;
    }
    
    mk11_obj.load_psf = arg_parse.load_psf;
    string oodle_folder = arg_parse.dll_folder;
    string file_in_name = arg_parse.file_name;

    FileHandle file;
    Compression* compression_obj;
    

    try
    {
        file.set(file_in_name);
        file.open_files();
    }
    catch (string error_string)
    {
        cerr<<"Couldn't open file "<<file_in_name<<"."<<endl;
        cerr<<error_string<<endl;
        return -1;
    }
    catch (...)
    {
        exception_ptr p = current_exception();
        try
        {
            rethrow_exception(p);
        }
        catch (const exception& e)
        {
            cerr<<e.what()<<endl;
        }
        return -2;
    }

    mk11_obj.register_file(file);

    
    try
    {
        _mkdir(mk11_obj.hFileObj->output_folder.c_str()); // output
        mk11_obj.read(mk11_obj.hFileObj->file_in);

        try
        {
            if (mk11_obj.info.compression_flag == MK11File::compression_flags::OODLE)
            {
                compression_obj = new Oodle(oodle_folder);
                cerr<<"Compression Type Set To Oodle"<<endl;
            }
            else if (mk11_obj.info.compression_flag == MK11File::compression_flags::ZLIB)
            {
                compression_obj = new Zlib("./zlib1.dll");
                cerr<<"Compression Type Set To Zlib"<<endl;
            }
            else
            {
                cerr<<"Unsupported Compression!";
                return -1;
            }
        }
        catch (string s)
        {
            cerr<<"Error "<<s<<endl;
            return 1;
        }

        _mkdir(mk11_obj.hFileObj->folder_out_name.c_str()); // Make Directory inside Output
        mk11_obj.hFileObj->open_files_out(); // Open Output files
        freopen(mk11_obj.hFileObj->file_out_info_name.c_str(), "w+", stdout); // Redirect stdout to .info file


        cout<<file<<endl<<endl;
        cerr<<file<<endl;

        cout<<mk11_obj<<endl;
        cout<<endl;

        /// Create UPK
        mk11_obj.hFileObj->file_out_upk<<mk11_obj;
        
        for (uint32_t i = 0; i < mk11_obj.info.number_of_packages; i++)
        {
            cout<<mk11_obj.packages[i]<<endl;
            string folder_out = mk11_obj.hFileObj->make_folder_out_name(i, mk11_obj.packages[i].name); // id_PackageName
            string params[2] = {mk11_obj.hFileObj->folder_out_name, folder_out};
            string mk_folder_out = mk11_obj.hFileObj->join(params, 2);
            _mkdir(mk_folder_out.c_str()); // Make Directory
            for (uint32_t j = 0; j < mk11_obj.packages[i].info.number_of_subpackages; j++)
            {
                cout<<mk11_obj.packages[i].subpackages[j]<<endl;
                cout<<mk11_obj.packages[i].subpackages[j].segment<<endl;

                // COALESCED DEC FILE
                stringstream coal_name;
                coal_name<<j<<".decompressed";
                string names[2] = {
                    mk_folder_out,
                    coal_name.str()
                };
                string dec_out_name = mk11_obj.hFileObj->join(names, 2);
                ofstream coal_out(dec_out_name.c_str(), ios::binary);
                for (uint32_t k = 0; k < mk11_obj.packages[i].subpackages[j].segment.compressed_segments_count; k++)
                {
                    cerr<<"\rDecompressing into "<<dec_out_name<<" ("<<k+1<<"/"
                        <<mk11_obj.packages[i].subpackages[j].segment.compressed_segments_count<<")";
                    cout<<mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k]<<endl;

                    mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k].read_data(mk11_obj.hFileObj->file_in);

                    // DECOMPRESSION HERE
                    CompressedSegment* c_seg = &mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k];
                    
                    char *out_data = new char [c_seg->info.compressed_segment_decompressed_size];
                    compression_obj->Decompress(c_seg->data, c_seg->info.compressed_segment_compressed_size, out_data,
                    c_seg->info.compressed_segment_decompressed_size);

                    for (uint64_t d = 0; d < c_seg->info.compressed_segment_decompressed_size; d++)
                    {
                        coal_out<<out_data[d];
                        mk11_obj.hFileObj->file_out_upk<<out_data[d]; //Write Dec Data into UPK File
                    }
                    coal_out.flush();

                    mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k].delete_data();
                    delete [] out_data;
                }
                cerr<<endl;
                coal_out.close();
            }
            
                
        }
        if (mk11_obj.number_of_extra_packages && mk11_obj.get_psf_status())
        {
            cout<<"Extra Packages: "<<endl;
            _mkdir(mk11_obj.hFileObj->folder_out_extra_name.c_str()); // Make Directory
            for (uint32_t i = 0; i < mk11_obj.number_of_extra_packages; i++)
            {
                cout<<mk11_obj.packages_extra[i]<<endl;
                string folder_out = mk11_obj.hFileObj->make_folder_out_name(i, mk11_obj.packages_extra[i].name); // id_ExtraPackageName
                string params[2] = {mk11_obj.hFileObj->folder_out_extra_name, folder_out};
                string mk_folder_out = mk11_obj.hFileObj->join(params, 2);
                _mkdir(mk_folder_out.c_str()); // Make Directory
                for (uint32_t j = 0; j < mk11_obj.packages_extra[i].info.number_of_subpackages; j++)
                {
                    cout<<mk11_obj.packages_extra[i].subpackages[j]<<endl;
                    cout<<mk11_obj.packages_extra[i].subpackages[j].segment<<endl;

                    // COALESCED DEC FILE
                    stringstream coal_name;
                    coal_name<<j<<".decompressed";
                    string names[2] = {
                        mk_folder_out,
                        coal_name.str()
                    };
                    string dec_out_name = mk11_obj.hFileObj->join(names, 2);
                    ofstream coal_out(dec_out_name.c_str(), ios::binary);
                    // cerr<<"Decompressing into "<<coal_name.str()<<endl;
                    
                    for (uint32_t k = 0; k < mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments_count; k++)
                    {
                        cerr<<"\rDecompressing into "<<dec_out_name<<" ("<<k+1<<"/"
                            <<mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments_count<<")";
                        cout<<mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k]<<endl;
                        mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k].read_data(mk11_obj.hFileObj->file_in_psf);

                        // DECOMPRESSION HERE
                        CompressedSegment* c_seg = &mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k];
                        
                        char *out_data = new char [c_seg->info.compressed_segment_decompressed_size];
                        compression_obj->Decompress(c_seg->data, c_seg->info.compressed_segment_compressed_size, out_data,
                        c_seg->info.compressed_segment_decompressed_size);

                        for (uint64_t d = 0; d < c_seg->info.compressed_segment_decompressed_size; d++)
                        {
                            coal_out<<out_data[d];
                        }
                        coal_out.flush();

                        delete [] out_data;
                        mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k].delete_data();
                    }
                    cerr<<endl;
                    coal_out.close();

                }
                    
            }
        }
        if (mk11_obj.number_of_extra_packages_tables)
        {
            cout<<"Extra Packages Table:"<<endl;
            mk11_obj.hFileObj->swap_table_additional();
            freopen(mk11_obj.hFileObj->file_out_table_name.c_str(), "w+", stdout);
            for (uint32_t i = 0; i < mk11_obj.number_of_extra_packages_tables; i++)
            {
                cout<<mk11_obj.psf_tables[i]<<endl;
            }
        }
        freopen("CONOUT$", "w", stdout);
        
        if (mk11_obj.number_of_bulk_packages_tables)
        {
            cout<<"Bulk Data Table:"<<endl;
            mk11_obj.hFileObj->swap_table_bulk();
            freopen(mk11_obj.hFileObj->file_out_table_name.c_str(), "w+", stdout);
            for (uint32_t i = 0; i < mk11_obj.number_of_bulk_packages_tables; i++)
            {
                cout<<mk11_obj.bulk_tables[i]<<endl;
            }
        }
        freopen("CONOUT$", "w", stdout);

        /// Start Reading UPK Here
        mk11_obj.hFileObj->swap_upk();

        // Read Tables
        mk11_obj.read_tables();

        // Read Name Table
        cout<<"Name Table:"<<endl;
        // mk11_obj.read_name_table();

        mk11_obj.hFileObj->swap_table_name();
        freopen(mk11_obj.hFileObj->file_out_table_name.c_str(), "w+", stdout);
        cout<<mk11_obj.name_table;
        freopen("CONOUT$", "w", stdout);

        // Read Export Table
        cout<<"Export Table:"<<endl;
        // mk11_obj.read_export_table();
        
        mk11_obj.hFileObj->swap_table_export();
        freopen(mk11_obj.hFileObj->file_out_table_name.c_str(), "w+", stdout);
        cout<<mk11_obj.export_table;
        freopen("CONOUT$", "w", stdout);

        // Read Import Table
        cout<<"Import Table: "<<endl;
        // mk11_obj.read_import_table();

        mk11_obj.hFileObj->swap_table_import();
        freopen(mk11_obj.hFileObj->file_out_table_name.c_str(), "w+", stdout);
        cout<<mk11_obj.import_table;
        freopen("CONOUT$", "w", stdout);

    }
    catch (string error_string)
    {
        cerr<<error_string<<endl;
        return 1;
    }
    catch (...)
    {
        exception_ptr p = current_exception();
        try
        {
            rethrow_exception(p);
        }
        catch (const exception& e)
        {
            cerr<<e.what()<<endl;
        }
        return -2;
    }
    
    return 0;
}

