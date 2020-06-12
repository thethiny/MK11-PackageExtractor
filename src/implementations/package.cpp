#include "package.h"

void Package::read(std::ifstream& fin)
{
    char name_len_arr[4];
    unsigned int read_size = sizeof(info);
    fin.read(name_len_arr, 4);

    memcpy(&name_len, name_len_arr, 4);
    name = new char[name_len];
    fin.read(name, name_len);

    char* read_arr = new char [read_size];

    fin.read(read_arr, read_size);
    memcpy(&info, read_arr, read_size);

    subpackages = new SubPackage[info.number_of_subpackages];

    for (uint32_t i = 0; i < info.number_of_subpackages; i++)
    {
        subpackages[i].read(fin);
        subpackages[i].id = i;
        
    }
    
}

std::ostream &operator<<(std::ostream& cout, Package obj)
{
    cout<<"Package #"<<obj.id<<":"<<std::endl;
    cout<<"\tName: "<<obj.name<<std::endl;
    cout<<"\tData Decompressed Offset: "<<obj.info.decompressed_offset<<std::endl;
    cout<<"\tDecompressed Size: "<<obj.info.decompressed_size<<std::endl;
    cout<<"\tStart Offset: "<<obj.info.start_offset<<std::endl;
    cout<<"\tSegment Size: "<<obj.info.segment_size<<std::endl;
    cout<<"\tSubPackages Count: "<<obj.info.number_of_subpackages;
    return cout;
}