#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<windows.h>
#include<direct.h>
using namespace std;

struct Package;
struct SubPackage;
struct Segment;
struct CompressedSegment;

/// Classes
struct Package{
    unsigned int name_len;
    unsigned int decompressed_size;
    unsigned int start_offset;
    unsigned int segment_size;
    unsigned int compressed_segments;
    unsigned int calculated_compressed_segments = 0;
    char* name;
    unsigned int id;
    SubPackage* subpackages;
};

struct SubPackage{
    unsigned int dec_size;
    unsigned int st_offset;
    unsigned int seg_size;
    unsigned int unkown;
    unsigned int id;
    Segment* segments;
};

struct Segment{
    unsigned int segment_compressed_size;
    unsigned int segment_decompressed_size;
    unsigned int segment_decompressed_sum = 0;
    unsigned int compressed_segments_count = 0;
    unsigned int id;
    CompressedSegment* compressed_segments;
};

struct CompressedSegment{
    unsigned int compressed_segment_compressed_size;
    unsigned int compressed_segment_decompressed_size;
    char* data;
    unsigned int id;
};

struct {
    char magic[4] = {0xC1u, 0x83u, 0x2Au, 0x9Eu};
    char file_version[2] = {0x01u, 0x03u};
    char engine_version[4] = {0xE7, 0x01};
    char script_version[2]= {0x9D, 0x00};
    char cook_version[4] = {0x50};
} MK11Package;

/// Globals
char mk_magic[4] = {0xC1u, 0x83u, 0x2Au, 0x9Eu};
char sup_fil_ver[2] = {0x01u, 0x03u};
char sup_eng_ver[4] = {0xE7, 0x01};
char sup_scr_ver[2]= {0x9D, 0x00};
char sup_cok_ver[4] = {0x50};

string out_name;

void read_package(ifstream& fin, unsigned int package_index, bool is_psf = false)
{
    char read_arr[8];
    unsigned int name_len;
    unsigned int decompressed_size;
    unsigned int unknown;
    unsigned int start_offset;
    unsigned int segment_size;
    unsigned int compressed_segments;
    unsigned int calculated_compressed_segments = 0;

    cout<<"Package "<<package_index+1<<":"<<endl;

    fin.read(read_arr, 4);
    memcpy(&name_len, read_arr, 4);
    char* name = new char [name_len];
    fin.read(name, name_len);
    cout<<"\tName: "<<name<<endl;

    fin.read(read_arr, 8); //Ignore
    memcpy(&unknown, read_arr, 4);
    cout<<"\tUnknown: "<<unknown<<endl;

    fin.read(read_arr, 8);
    memcpy(&decompressed_size, read_arr, 4);
    cout<<"\tDecompressed Size: "<<decompressed_size<<endl;

    fin.read(read_arr, 8);
    memcpy(&start_offset, read_arr, 4);
    start_offset = start_offset;
    cout<<"\tStart Offset: "<<start_offset<<endl;

    fin.read(read_arr, 8);
    memcpy(&segment_size, read_arr, 4);
    cout<<"\tSegment Size: "<<segment_size<<endl;

    fin.read(read_arr, 4);
    memcpy(&compressed_segments, read_arr, 4);
    cout<<"\tCompressed Segments: "<<compressed_segments<<endl;


    unsigned int calculated_size = 0;
    cout<<"\tDecompressed Size: "<<decompressed_size<<endl;
    while (calculated_size < decompressed_size) //For Each SubPackage
    {
        unsigned int dec_size;
        unsigned int st_offset;
        unsigned int seg_size;
        unsigned int unkown;

        cout<<"\tSegment: "<<calculated_compressed_segments+1<<endl;

        fin.read(read_arr, 8);
        memcpy(&unkown, read_arr, 8);
        cout<<"\t\tUnk: "<<unkown<<endl;

        fin.read(read_arr, 8);
        memcpy(&dec_size, read_arr, 4);
        cout<<"\t\tDecompressed Size: "<<dec_size<<endl;

        fin.read(read_arr, 8);
        memcpy(&st_offset, read_arr, 4);
        cout<<"\t\tStart Offset: "<<st_offset<<endl;

        fin.read(read_arr, 8);
        memcpy(&seg_size, read_arr, 4);
        cout<<"\t\tSegment Size: "<<seg_size<<endl;

        if (!is_psf)
        {
            unsigned long long cur_position = fin.tellg();
            
            fin.seekg(st_offset);
            char seg_magic[4];
            fin.read(seg_magic, 4);
            if (memcmp(seg_magic, mk_magic, 4))
            {
                cerr<<"Invalid Segment. Skipping..."<<endl;
            }
            else
            {
                unsigned int segment_compressed_size;
                unsigned int segment_decompressed_size;
                unsigned int segment_decompressed_sum = 0;
                unsigned int compressed_segments = 0;

                fin.read(read_arr, 4); // Should be 0
                fin.read(read_arr, 4); // Lower 2 bytes contain 02 00 which mean package I think
                fin.read(read_arr, 4); // Padding
                
                fin.read(read_arr, 8);
                memcpy(&segment_compressed_size, read_arr, 4);
                cout<<"\t\tSegment Compressed Size: "<<segment_compressed_size<<endl;

                fin.read(read_arr, 8);
                memcpy(&segment_decompressed_size, read_arr, 4);
                cout<<"\t\tSegment Decompressed Size: "<<segment_decompressed_size<<endl;

                while (segment_decompressed_sum < segment_decompressed_size) // For each Compressed Segment
                {
                    cout<<"\t\tCompressed Segment: "<<compressed_segments+1<<endl;
                    unsigned int compressed_segment_compressed_size;
                    unsigned int compressed_segment_decompressed_size;

                    fin.read(read_arr, 8);
                    memcpy(&compressed_segment_compressed_size, read_arr, 4);
                    cout<<"\t\t\tCompressed Size: "<<compressed_segment_compressed_size<<endl;

                    fin.read(read_arr, 8);
                    memcpy(&compressed_segment_decompressed_size, read_arr, 4);
                    cout<<"\t\t\tDecompressed Size: "<<compressed_segment_decompressed_size<<endl;

                    segment_decompressed_sum += compressed_segment_decompressed_size;
                    compressed_segments++;
                }
                cout<<"\t\tTotal Compressed Segments Found: "<<compressed_segments<<endl;
                unsigned int c_s_st_pos = fin.tellg();
                cout<<"\t\tCompressed Segments Start Position: "<<c_s_st_pos<<endl;

                fin.seekg(st_offset + 0x20);

                unsigned int compressed_offset = c_s_st_pos;

                segment_decompressed_sum = 0;
                for (int k = 0; k < compressed_segments; k++)
                {
                    cout<<"\t\tExtracting Compressed Segment: "<<k+1<<endl;
                    unsigned int compressed_segment_compressed_size;
                    unsigned int compressed_segment_decompressed_size;

                    fin.read(read_arr, 8);
                    memcpy(&compressed_segment_compressed_size, read_arr, 4);

                    fin.read(read_arr, 8);
                    memcpy(&compressed_segment_decompressed_size, read_arr, 4);

                    stringstream out_file_name;
                    out_file_name<<out_name<<package_index+1<<"_"<<name<<"_"<<calculated_compressed_segments+1<<"_"<<k+1<<".cmp";
                    ofstream fout(out_file_name.str().c_str(), ios::binary);

                    char *data = new char [compressed_segment_compressed_size];

                    unsigned int cur_position_ext = fin.tellg();

                    fin.seekg(compressed_offset);
                    compressed_offset += compressed_segment_compressed_size;

                    fin.read(data, compressed_segment_compressed_size);

                    fin.seekg(cur_position_ext);

                    cout<<"\t\t\tCompression Type: ";
                    for (int j = 0; j < 2; j++)
                        cout<<hex<<setfill('0')<<setw(2)<<(unsigned int)(unsigned char)data[j]<<" ";
                    cout<<endl;

                    unsigned long long AND = 0x00000000000000FF;
                    for (int j = 0; j < 8; j++)
                    {
                        fout<<(unsigned char)(((unsigned long long)compressed_segment_decompressed_size & AND)>>(8*j));
                        AND<<=8;
                    }
                    for (int j = 0; j < compressed_segment_compressed_size; j++)
                        fout<<data[j];

                }                            



            }

            fin.seekg(cur_position); //Return to previous location
        }

        calculated_size += dec_size;
        calculated_compressed_segments++;
    }
    if (calculated_size > decompressed_size || calculated_compressed_segments > compressed_segments)
    {
        cerr<<"Warning: Decompressed Data Overflow."<<endl;
        cerr<<"Expected size: "<<decompressed_size<<" but got "<<calculated_size<<endl;
        cerr<<"Expected Segments: "<<compressed_segments<<" but got "<<calculated_compressed_segments<<endl;
    }
    cout<<"\tCalculated #Segments: "<<calculated_compressed_segments<<endl;



}

int main(int argc, const char* argv[])
{
    if (argc < 2)
    {
        cerr<<"Usage: "<<argv[0]<<" file_to_extract";
        return -1;
    }
    ifstream fin;
    fin.open(argv[1], ios::binary);
    fin>>noskipws;

    if (!fin)
    {
        cerr<<"Error opening file "<<argv[1]<<endl;
        return -1;
    }

    out_name = argv[1] + string("_out\\");
    cout<<"File Name: "<<out_name<<endl;
    _mkdir(out_name.c_str());


    char read_arr[8] = {0};

    char file_magic[4];
    char file_version[2];
    char engine_version[4];
    char script_version[2];
    char cooking_version[4];
    char game_package[4];

    char package_type = 0; //0 None, 1 MK11 Package, 2 PSF Package, 3 MKO Package (unsup)

    fin.read(file_magic, 4);
    cout<<endl;
    if (memcmp(mk_magic, file_magic, 4))
    {
        cerr<<"Not a Mortal Kombat 11 Package file";
        return -1;
    }

    fin.read(file_version, 2);
    fin.read(script_version, 2);
    fin.seekg(0x10);
    fin.read(engine_version, 4);
    fin.read(game_package, 4);
    fin.read(cooking_version, 4);

    if (memcmp(sup_fil_ver, file_version, 2))
    {
        char psf_ver[2] = {0};
        if (memcmp(file_version, psf_ver, 2))
        {
            cerr<<"Not a supported file version.";
            return -1;
        }
        else
        {
            cout<<"Compressed Data Package";
            package_type = 2;
        }
        
        
    }
    else
    {
        package_type = 1;
    }
    
    if (package_type == 1) // MK11 Package
    {
        if (memcmp(sup_scr_ver, script_version, 2))
        {
            cerr<<"Not a supported file version.";
            return -1;
        }

        if (memcmp(sup_eng_ver, engine_version, 2))
        {
            cerr<<"Not a supported file version.";
            return -1;
        }

        if (memcmp(sup_cok_ver, cooking_version, 2))
        {
            cerr<<"Not a supported file version.";
            return -1;
        }

        fin.seekg(0x68);

        unsigned int packages_count;
        fin.read(read_arr, 4);
        memcpy(&packages_count, read_arr, 4);
        cout<<hex<<"Number of Packages: "<<packages_count<<endl;

        for (int i = 0; i < packages_count; i++)
        {
            read_package(fin, i, false);
        }

        unsigned int additional_packages_count;

        fin.read(read_arr, 4); //Check for additional packages
        memcpy(&additional_packages_count, read_arr, 4);
        if (additional_packages_count)
        {
            cout<<"Additional "<<additional_packages_count<<" Packages Exist in .PSF file"<<endl;
            for (int i = 0; i < additional_packages_count; i++)
            {
                read_package(fin, i+packages_count, true);
            }
            
        }

        fin.seekg(fin.tellg()+0x18); //Skip Padding

        unsigned int file_internal_name_len;
        fin.read(read_arr, 4);
        memcpy(&file_internal_name_len, read_arr, 4);
        char* file_internal_name = new char[file_internal_name_len];
        fin.read(file_internal_name, file_internal_name_len);

        cout<<"Internal Package Name: "<<file_internal_name<<endl;


        

    }





}