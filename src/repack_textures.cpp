#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <direct.h>
#include <windows.h>
#include <direct.h>

#include "compression.h"
#include "oodle.h"
#include "mk11file.h"
#include "package.h"
#include "subpackage.h"
#include "segment_info.h"
#include "compressedsegment.h"
#include "filehandle.h"

using namespace std;


string get_folder_name(string name)
{
    int index = -1;
    for (int i = 0; i < name.length(); i++)
    {
        if (name[i] == '_')
        {
            index = i;
            break;
        }
            
    }
    if (index < -1)
        return name;
    else
        return name.substr(index+1, name.length());
}

uint64_t get_number_of_textures(string path)
{
    path += "*.decompressed";
    WIN32_FIND_DATA data;
    HANDLE hFind;
    uint64_t ctr = 0;
    if ((hFind = FindFirstFile(path.c_str(), &data)) != INVALID_HANDLE_VALUE)
    {
        do {
            ctr++;
        } while (FindNextFile(hFind, &data) != 0);
    }

    return ctr;
}

int main(int argv, const char* argc[])
{
    if (argv < 2)
    {
        cerr<<"Invalid Usage";
        return -1;
    }

    MK11File mk11_file;
    FileHandle filehandle;
    const char* file_in_name = argc[1];
    filehandle.set(file_in_name);
    mk11_file.register_file(filehandle);

    string repack_folder_name = "repack";
    mkdir(repack_folder_name.c_str());
    string file_base_name = mk11_file.hFileObj->get_base_name(file_in_name);
    string out_psf_name = file_base_name + ".PSF";
    string out_psftable_name = file_base_name + ".PSFTable";
    string out_headertable_name = file_base_name + ".PSFHeader";
    ///Scope
    {
        string params[2] = {
            repack_folder_name,
            out_psf_name
        };
        out_psf_name = mk11_file.hFileObj->join(params, 2);
        params[1] = out_psftable_name;
        out_psftable_name = mk11_file.hFileObj->join(params, 2);
        params[1] = out_headertable_name;
        out_headertable_name = mk11_file.hFileObj->join(params, 2);
    }


    string folders[2] = {
        "0_Mesh",
        "1_Texture"
    };
    Compression* compression_obj = new Oodle("..\\..\\Binaries\\Retail\\oo2core_5_win64.dll");
    uint64_t max_dec_size = 0x20000;
    uint64_t magic = 0x9E2A83C1;
    ofstream fout(out_psf_name.c_str(), ios::binary);
    ofstream tabledata(out_psftable_name.c_str(), ios::binary);

    // Scope
    string table_in_name;
    {
        string params[2] = {
        mk11_file.hFileObj->folder_out_extra_name,
        "PSFTable.bin"
        };
        table_in_name = mk11_file.hFileObj->join(params, 2);
    }
    
    ifstream table_in(table_in_name.c_str(), ios::binary);
    table_in>>noskipws;

    uint32_t folders_count = 2;

    mk11_file.number_of_extra_packages = folders_count;
    mk11_file.packages_extra = new Package [folders_count];

    uint64_t total_compressed_size = 0;
    uint64_t total_decompressed_size = 0;
    uint64_t total_file_size = 0;
    uint64_t zero64 = 0;
    uint64_t total_compressed_offset = 0;
    uint64_t total_decompressed_offset = 0;

    for (int i = 0; i < folders_count; i++)
    {
        string folder_name = get_folder_name(folders[i]);
        uint32_t folder_name_len = folder_name.length() +1;
        stringstream folder;
        folder<<mk11_file.hFileObj->folder_out_extra_name<<"\\"<<folders[i]<<"\\";
        ///
        uint64_t files_count = get_number_of_textures(folder.str());
        mk11_file.packages_extra[i].name_len = folder_name_len;
        mk11_file.packages_extra[i].name = new char [folder_name_len];
        for (uint32_t idx = 0; idx < folder_name_len -1; idx++)
        {
            mk11_file.packages_extra[i].name[idx] = folder_name[idx];
        }
        mk11_file.packages_extra[i].name[folder_name_len-1] = '\0';
        mk11_file.packages_extra[i].subpackages = new SubPackage [files_count];
        mk11_file.packages_extra[i].info.decompressed_offset = total_decompressed_offset;
        mk11_file.packages_extra[i].info.decompressed_size;
        mk11_file.packages_extra[i].info.start_offset = total_compressed_offset;
        mk11_file.packages_extra[i].info.segment_size = 0;
        mk11_file.packages_extra[i].info.number_of_subpackages = files_count;
        
        uint64_t file_idx = 0;
        uint64_t next_table = 0;
        stringstream file;
        file<<folder.str()<<file_idx<<".decompressed";
        ifstream fin(file.str().c_str(), ios::binary);
        fin>>noskipws;
        cout<<"Folder: "<<folder.str()<<endl;
        cout<<"File: "<<file.str()<<endl;
        while(fin.good())
        {
            fin.seekg(0, ios::end);
            uint64_t file_size = fin.tellg();
            cout<<"Operating on "<<file_size<<" bytes."<<endl;
            cout<<"File "<<file_idx<<"/"<<files_count<<endl;

            /// PSF Table Stuff
            if (file_idx == next_table)
            {
                char read_array[8];
                uint64_t entry_id;
                uint32_t entries_count;
                uint32_t name_len;
                table_in.read(read_array, 8);
                memcpy(&entry_id, read_array, 8);
                table_in.read(read_array, 4);
                memcpy(&entries_count, read_array, 4);
                table_in.read(read_array, 4);
                memcpy(&name_len, read_array, 4);
                char* name = new char [name_len];
                table_in.read(name, name_len);
                next_table += entries_count;
                tabledata.write((char*)&entry_id, 8);
                tabledata.write((char*)&name_len, 4);
                tabledata.write(name, name_len);
                tabledata.write((char*)&entries_count, 4);

            }


            fin.seekg(0, ios::beg);
            uint64_t segments = 0;
            while (true and file_size) //if size 0 then no need
            {
                segments++;
                if (file_size <= segments*max_dec_size)
                {
                    break;
                }
            }
            mk11_file.packages_extra[i].subpackages[file_idx].segment.compressed_segments_count = segments;
            mk11_file.packages_extra[i].subpackages[file_idx].segment.compressed_segments = new CompressedSegment [segments];
            mk11_file.packages_extra[i].subpackages[file_idx].segment.info.max_seg_dec_size = max_dec_size;
            mk11_file.packages_extra[i].subpackages[file_idx].segment.info.segment_compressed_size = 0;
            mk11_file.packages_extra[i].subpackages[file_idx].segment.info.segment_decompressed_size = file_size;
            mk11_file.packages_extra[i].subpackages[file_idx].info.decompressed_offset = total_decompressed_offset;
            mk11_file.packages_extra[i].subpackages[file_idx].info.st_offset = total_compressed_offset;
            mk11_file.packages_extra[i].subpackages[file_idx].info.dec_size = file_size;

            mk11_file.packages_extra[i].info.decompressed_size += file_size;
            total_decompressed_offset += file_size;
            
            mk11_file.packages_extra[i].subpackages[file_idx].info.seg_size = sizeof(SegmentInfo::Info);
            mk11_file.packages_extra[i].subpackages[file_idx].info.seg_size += segments * sizeof(CompressedSegment::Info);
            total_compressed_offset += sizeof(SegmentInfo::Info);
            total_compressed_offset += segments * sizeof(CompressedSegment::Info);

            fout.write((char*)&magic, 8);
            fout.write((char*)&max_dec_size, 8);
            uint64_t zero = 0;
            fout.write((char*)&zero, 8);
            fout.write((char*)&file_size, 8);
            for (uint64_t j = 0; j < segments; j++)
            {
                fout.write((char*)&zero, 8);
                uint64_t offset = j*max_dec_size;
                uint64_t remaining = min(max_dec_size, file_size - offset);
                fout.write((char*)&remaining, 8);
            }
            fout.flush();
            char* data = new char [file_size];
            fin.read(data, file_size);
            uint64_t total_com_size = 0;
            for (uint64_t j = 0; j < segments; j++)
            {
                uint64_t offset = j*max_dec_size;
                uint64_t remaining = min(max_dec_size, file_size - offset);
                char* com_data = new char[max_dec_size + 0x10000];
                int64_t com_size = compression_obj->Compress(data + (j*max_dec_size), remaining, com_data);
                total_com_size += com_size;
                fout.write(com_data, com_size);
                delete [] com_data;

                ///ReWrite Header
                fout.seekp((int64_t)(-total_com_size), ios::cur);
                fout.seekp((int64_t)(-(0x10*(segments-j))), ios::cur);
                fout.write((char*)&com_size, 8);
                fout.seekp(0, ios::end);
                ///End ReWrtie Header

                mk11_file.packages_extra[i].subpackages[file_idx].segment.compressed_segments[j].info.compressed_segment_compressed_size = com_size;
                mk11_file.packages_extra[i].subpackages[file_idx].segment.compressed_segments[j].info.compressed_segment_decompressed_size = remaining;
                mk11_file.packages_extra[i].subpackages[file_idx].segment.info.segment_compressed_size += com_size;
                mk11_file.packages_extra[i].subpackages[file_idx].info.seg_size += com_size;
                total_compressed_offset += com_size;                
            }
            mk11_file.packages_extra[i].info.segment_size += mk11_file.packages_extra[i].subpackages[file_idx].info.seg_size;
            ///ReWrite Header
            fout.seekp((int64_t)(-total_com_size), ios::cur);
            fout.seekp((int64_t)(-(0x10*segments)), ios::cur);
            fout.seekp(-0x10, ios::cur);
            fout.write((char*)&total_com_size, 8);
            fout.seekp(0, ios::end);
            ///End ReWrite Header
            fin.close();
            fout.flush();
            file_idx++;
            file.str(string());
            file.clear();
            file<<folder.str()<<file_idx<<".decompressed";
            fin.open(file.str().c_str(), ios::binary);
            fin>>noskipws;

            ///PSF Table Stuff Again
            tabledata.write((char*)&file_size, 8);
            tabledata.write((char*)&mk11_file.packages_extra[i].subpackages[file_idx-1].info.seg_size, 8);
            uint64_t table_data_offset = total_compressed_offset - mk11_file.packages_extra[i].subpackages[file_idx-1].info.seg_size;
            tabledata.write((char*)&table_data_offset, 8);
            tabledata.write((char*)&table_data_offset, 8);
            if (file_idx == next_table)
            {
                uint32_t c_flag = 0x100;
                tabledata.write((char*)&c_flag, 4);
            }
        }

    }

    /// Write HeaderData Sections
    ofstream headerdata(out_headertable_name.c_str(), ios::binary);
    headerdata.write((char*)&mk11_file.number_of_extra_packages, 4);
    for (uint32_t i = 0; i < mk11_file.number_of_extra_packages; i++)
    {
            headerdata.write((char*)&mk11_file.packages_extra[i].name_len, 4);
            headerdata.write(mk11_file.packages_extra[i].name, mk11_file.packages_extra[i].name_len);
            headerdata.write((char*)&mk11_file.packages_extra[i].info.decompressed_offset, 8);
            headerdata.write((char*)&mk11_file.packages_extra[i].info.decompressed_size, 8);
            headerdata.write((char*)&mk11_file.packages_extra[i].info.start_offset, 8);
            headerdata.write((char*)&mk11_file.packages_extra[i].info.segment_size, 8);
            headerdata.write((char*)&mk11_file.packages_extra[i].info.number_of_subpackages, 4);
        for (uint32_t j = 0; j < mk11_file.packages_extra[i].info.number_of_subpackages; j++)
        {
            headerdata.write((char*)&mk11_file.packages_extra[i].subpackages[j].info.decompressed_offset, 8);
            headerdata.write((char*)&mk11_file.packages_extra[i].subpackages[j].info.dec_size, 8);
            headerdata.write((char*)&mk11_file.packages_extra[i].subpackages[j].info.st_offset, 8);
            headerdata.write((char*)&mk11_file.packages_extra[i].subpackages[j].info.seg_size, 8);
        }
    }
}