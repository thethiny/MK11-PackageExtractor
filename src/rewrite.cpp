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
#include "oodle.h"

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

    Oodle* oodle_obj;

    try
    {
        oodle_obj = new Oodle(oodle_folder);
    }
    catch (string s)
    {
        cerr<<"Error "<<s<<endl;
        return 1;
    }
    

    try
    {
        file.set(file_in_name);
        file.open_files();
    }
    catch (string error_string)
    {
        cerr<<"Couldn't open file "<<argc[1]<<".";
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

    cout<<file<<endl<<endl;
    cerr<<file<<endl;
    
    try
    {
        _mkdir(mk11_obj.input_file_obj->output_folder.c_str()); // output
        mk11_obj.read(mk11_obj.input_file_obj->file_in);
        cout<<mk11_obj<<endl;
        cout<<endl;
        _mkdir(mk11_obj.input_file_obj->folder_out_name.c_str()); // Make Directory
        for (uint32_t i = 0; i < mk11_obj.info.number_of_packages; i++)
        {
            cout<<mk11_obj.packages[i]<<endl;
            string folder_out = mk11_obj.input_file_obj->make_folder_out_name(i, mk11_obj.packages[i].name); // id_PackageName
            string params[2] = {mk11_obj.input_file_obj->folder_out_name, folder_out};
            string mk_folder_out = mk11_obj.input_file_obj->join(params, 2);
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
                string dec_out_name = mk11_obj.input_file_obj->join(names, 2);
                ofstream coal_out(dec_out_name.c_str(), ios::binary);
                for (uint32_t k = 0; k < mk11_obj.packages[i].subpackages[j].segment.compressed_segments_count; k++)
                {
                    cerr<<"\rDecompressing into "<<dec_out_name<<" ("<<k+1<<"/"
                        <<mk11_obj.packages[i].subpackages[j].segment.compressed_segments_count<<")";
                    cout<<mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k]<<endl;
                    // string file_out = mk11_obj.input_file_obj->make_file_out_name(j, k); //SubpackageId_SegmentId.oodle
                    // std::string names[2] = {
                    //     mk_folder_out,
                    //     file_out
                    // };
                    // std::string out_f = mk11_obj.input_file_obj->join(names, 2);
                    // cerr<<"Extracting file to "<<out_f<<endl;
                    mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k].read_data(mk11_obj.input_file_obj->file_in);

                    // ofstream fout(out_f.c_str(), ios::binary);
                    // fout<<mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k];
                    // fout.flush();
                    // fout.close();

                    // DECOMPRESSION HERE
                    CompressedSegment* c_seg = &mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k];
                    // std::string out_f_dec = mk11_obj.input_file_obj->join(names, 2) + string(".dec");
                    // ofstream fout_dec(out_f_dec.c_str(), ios::binary);
                    
                    char *out_data = new char [c_seg->info.compressed_segment_decompressed_size];
                    oodle_obj->Decompress(c_seg->data, c_seg->info.compressed_segment_compressed_size, out_data,
                    c_seg->info.compressed_segment_decompressed_size);
                    // cerr<<"Decompressed"<<endl;

                    for (uint64_t d = 0; d < c_seg->info.compressed_segment_decompressed_size; d++)
                    {
                        // fout_dec<<out_data[d];
                        coal_out<<out_data[d];
                    }
                    // fout_dec.flush();
                    // fout_dec.close();
                    coal_out.flush();

                    // // TESTING RECOMPRESSION HERE
                    // std::string out_f_enc = mk11_obj.input_file_obj->join(names, 2) + string(".dec.oodl");
                    // char* cmp_data = new char [c_seg->info.compressed_segment_decompressed_size + 0x10000];
                    // ofstream fout_enc(out_f_enc.c_str(), ios::binary);
                    // int64_t out_size = oodle_obj->Compress(out_data, c_seg->info.compressed_segment_decompressed_size, cmp_data);
                    // uint8_t* size_array = new uint8_t[8];
                    // btoa(size_array, c_seg->info.compressed_segment_decompressed_size, 8);
                    // for (uint64_t d = 0; d < 8; d++)
                    // {
                    //     fout_enc<<size_array[d];
                    // }
                    // for (uint64_t d = 0; d < out_size; d++)
                    // {
                    //     fout_enc<<cmp_data[d];
                    // }
                    // cerr<<"Compressed"<<endl;
                    // fout_enc.close();
                    mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k].delete_data();
                    delete [] out_data;
                    // delete [] cmp_data;
                }
                cerr<<endl;
                coal_out.close();
            }
            
                
        }
        if (mk11_obj.number_of_extra_packages && mk11_obj.get_psf_status())
        {
            cout<<"Extra Packages: "<<endl;
            _mkdir(mk11_obj.input_file_obj->folder_out_extra_name.c_str()); // Make Directory
            for (uint32_t i = 0; i < mk11_obj.number_of_extra_packages; i++)
            {
                cout<<mk11_obj.packages_extra[i]<<endl;
                string folder_out = mk11_obj.input_file_obj->make_folder_out_name(i, mk11_obj.packages_extra[i].name); // id_ExtraPackageName
                string params[2] = {mk11_obj.input_file_obj->folder_out_extra_name, folder_out};
                string mk_folder_out = mk11_obj.input_file_obj->join(params, 2);
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
                    string dec_out_name = mk11_obj.input_file_obj->join(names, 2);
                    ofstream coal_out(dec_out_name.c_str(), ios::binary);
                    // cerr<<"Decompressing into "<<coal_name.str()<<endl;
                    
                    for (uint32_t k = 0; k < mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments_count; k++)
                    {
                        cerr<<"\rDecompressing into "<<dec_out_name<<" ("<<k+1<<"/"
                            <<mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments_count<<")";
                        cout<<mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k]<<endl;
                        // string file_out = mk11_obj.input_file_obj->make_file_out_name(j, k); //SubpackageId_SegmentId.oodle
                        // std::string names[2] = {
                        //     mk_folder_out,
                        //     file_out
                        // };
                        // std::string out_f = mk11_obj.input_file_obj->join(names, 2);
                        // cerr<<"Extracting file to "<<out_f<<endl;
                        mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k].read_data(mk11_obj.input_file_obj->file_in_psf);
                        // ofstream fout(out_f.c_str(), ios::binary);
                        // fout<<mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k];
                        // fout.flush();
                        // fout.close();

                        // DECOMPRESSION HERE
                        CompressedSegment* c_seg = &mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k];
                        std::string out_f_dec = mk11_obj.input_file_obj->join(names, 2) + string(".dec");
                        ofstream fout_dec(out_f_dec.c_str(), ios::binary);
                        
                        char *out_data = new char [c_seg->info.compressed_segment_decompressed_size];
                        oodle_obj->Decompress(c_seg->data, c_seg->info.compressed_segment_compressed_size, out_data,
                        c_seg->info.compressed_segment_decompressed_size);
                        // cerr<<"Decompressed"<<endl;

                        for (uint64_t d = 0; d < c_seg->info.compressed_segment_decompressed_size; d++)
                        {
                            // fout_dec<<out_data[d];
                            coal_out<<out_data[d];
                        }
                        // fout_dec.flush();
                        // fout_dec.close();
                        coal_out.flush();

                        delete [] out_data;
                        mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k].delete_data();
                    }
                    cerr<<endl;
                    coal_out.close();

                }
                    
            }
        }

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

