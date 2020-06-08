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

using namespace std;

int main(int argv, const char* argc[])
{
    if (argv < 2)
    {
        cerr<<"Usage: "<<endl<<"\t"<<argc[0]<<" FileName.XXX"<<endl;
        return 1;
    }
    MK11File mk11_obj = MK11File();
    if (argv > 2 && argc[2] == "0")
    {
        mk11_obj.load_psf = false;
    }

    FileHandle file;

    try
    {
        file.set(argc[1]);
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

    mk11_obj.register_file(file); //Change this so you open file in FileHandle

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
                for (uint32_t k = 0; k < mk11_obj.packages[i].subpackages[j].segment.compressed_segments_count; k++)
                {
                    cout<<mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k]<<endl;
                    string file_out = mk11_obj.input_file_obj->make_file_out_name(j, k); //SubpackageId_SegmentId.oodle
                    std::string names[2] = {
                        mk_folder_out,
                        file_out
                    };
                    std::string out_f = mk11_obj.input_file_obj->join(names, 2);
                    cerr<<"Extracting file to "<<out_f<<endl;
                    mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k].read_data(mk11_obj.input_file_obj->file_in);
                    ofstream fout(out_f.c_str(), ios::binary);
                    fout<<mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k];
                    mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k].delete_data();
                }
            }
            
                
        }
        if (mk11_obj.number_of_extra_packages)
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
                    if (mk11_obj.get_psf_status())
                    {
                        cout<<mk11_obj.packages_extra[i].subpackages[j].segment<<endl;
                        for (uint32_t k = 0; k < mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments_count; k++)
                        {
                            cout<<mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k]<<endl;
                            string file_out = mk11_obj.input_file_obj->make_file_out_name(j, k); //SubpackageId_SegmentId.oodle
                            std::string names[2] = {
                                mk_folder_out,
                                file_out
                            };
                            std::string out_f = mk11_obj.input_file_obj->join(names, 2);
                            cerr<<"Extracting file to "<<out_f<<endl;
                            mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k].read_data(mk11_obj.input_file_obj->file_in_psf);
                            ofstream fout(out_f.c_str(), ios::binary);
                            fout<<mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k];
                            mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k].delete_data();
                        }
                    }

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

