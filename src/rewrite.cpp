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

class FileHandle{
    public:
        string file_in_name;
        string file_out_name;
        string file_in_psf_name;
        string files_path = "files\\";
        string testing_files[3] = {
            "BARAKA_A_CHARINTRO_ScriptAssets.XXX",
            "GEARASSETS_KIT_ScriptAssets.XXX",
            "Init.XXX"
        };

    FileHandle(string fname)
    {
        set(fname);
    }

    void set(string fname)
    {
        file_in_name = fname;
        file_in_psf_name = fname.substr(0, fname.length()-4) + ".psf";
    }

    void print()
    {
        cout<<"File Name: "<<file_in_name<<endl;
        cout<<"PSF File Name: "<<file_in_psf_name<<endl;
    }

    string get_file_path(string fname)
    {
        return (files_path + fname);
    }

};


int main(int argv, const char* argc[])
{
    MK11File mk11_obj = MK11File();
    FileHandle file("Init.xxx");
    string file_name = file.testing_files[1];

    cout<<file.get_file_path(file_name).c_str()<<endl;

    ifstream fin(file.get_file_path(file_name).c_str(), ios::binary);
    fin>>noskipws;


    if (!fin.good())
    {
        cerr<<"Couldn't open file";
        return -1;
    }
    
    try
    {
        mk11_obj.read(fin);
        cout<<mk11_obj<<endl;
        cout<<endl;
        for (uint32_t i = 0; i < mk11_obj.info.number_of_packages; i++)
        {
            cout<<mk11_obj.packages[i]<<endl;
            for (uint32_t j = 0; j < mk11_obj.packages[i].info.number_of_subpackages; j++)
            {
                cout<<mk11_obj.packages[i].subpackages[j]<<endl;
                cout<<mk11_obj.packages[i].subpackages[j].segment<<endl;
                for (uint32_t k = 0; k < mk11_obj.packages[i].subpackages[j].segment.compressed_segments_count; k++)
                {
                    cout<<mk11_obj.packages[i].subpackages[j].segment.compressed_segments[k]<<endl;
                }
            }
            
                
        }
        cout<<"Extra Packages: "<<endl;
        for (uint32_t i = 0; i < mk11_obj.number_of_extra_packages; i++)
        {
            cout<<mk11_obj.packages_extra[i]<<endl;
            for (uint32_t j = 0; j < mk11_obj.packages_extra[i].info.number_of_subpackages; j++)
            {
                cout<<mk11_obj.packages_extra[i].subpackages[j]<<endl;
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

