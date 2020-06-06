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
    FileHandle file(argc[1]);
    mk11_obj.register_file(file);
    string file_name = mk11_obj.input_file_obj->get_file_path(argc[1]);

    cout<<file<<endl;
    cerr<<file<<endl;

    ifstream fin(file_name.c_str(), ios::binary);
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
                if (mk11_obj.get_psf_status())
                {
                    cout<<mk11_obj.packages_extra[i].subpackages[j].segment<<endl;
                    for (uint32_t k = 0; k < mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments_count; k++)
                    {
                        cout<<mk11_obj.packages_extra[i].subpackages[j].segment.compressed_segments[k]<<endl;
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

