#include "extra_table_entry.h"

void ExtraTableEntry::read(std::ifstream& fin)
{
    char* read_array = new char [sizeof(info)];
    fin.read(read_array, sizeof(info));
    memcpy(&info, read_array, sizeof(info));
}

std::ostream& operator<<(std::ostream& cout, ExtraTableEntry obj)
{
    cout<<"\t\tEntry #"<<obj.id<<std::endl;
    cout<<"\t\t\tDecompressed Size: "<<obj.info.decompressed_size<<std::endl;
    cout<<"\t\t\tCompressed Size: "<<obj.info.compressed_size<<std::endl;
    cout<<"\t\t\tDecompressed Offset: "<<obj.info.decompressed_offset<<std::endl;
    cout<<"\t\t\tCompressed Offset: "<<obj.info.compressed_offset<<std::endl;

    return cout;
}

std::ofstream& operator<<(std::ofstream& fout, ExtraTableEntry obj)
{
    fout.write((char*)&obj.info, sizeof(obj.info));
    return fout;
}