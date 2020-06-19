#include "extra_table.h"

void ExtraTable::read(std::ifstream &fin)
{
    char* read_array = new char [sizeof(header)];
    fin.read(read_array, sizeof(header));
    memcpy(&header, read_array, sizeof(header));

    name = new char [header.name_len];
    fin.read(name, header.name_len);

    fin.read(read_array, 4);
    memcpy(&entries_count, read_array, 4);

    entries = new ExtraTableEntry [entries_count];
    for (uint32_t i = 0; i < entries_count; i++)
    {
        entries[i].read(fin);
        entries[i].id = i;
    }

    fin.read(read_array, 4);
    memcpy(&compression_flag, read_array, 4);
}

std::ostream& operator<<(std::ostream& cout, ExtraTable obj)
{
    cout<<"\tTable #"<<obj.id<<std::endl;
    cout<<"\t\tUnk1: "<<obj.header.unk1<<std::endl;
    cout<<"\t\tUnk2: "<<obj.header.unk2<<std::endl;
    cout<<"\t\tName: "<<obj.name<<std::endl;
    cout<<"\t\tNumber of Table Entries: "<<obj.entries_count<<std::endl;
    for (uint32_t i = 0; i < obj.entries_count; i++)
    {
        cout<<obj.entries[i];
    }
    cout<<"\t\tCompression Flag: "<<obj.compression_flag;

    return cout;

}

std::ofstream& operator<<(std::ofstream& fout, ExtraTable obj)
{
    fout.write((char*)&obj.header, sizeof(obj.header));
    uint32_t name_len = strlen(obj.name) +1;
    fout.write(obj.name, name_len);
    fout.write((char*)&obj.entries_count, sizeof(obj.entries_count));

    for (uint32_t i = 0; i < obj.entries_count; i++)
        fout<<obj.entries[i];

    fout.write((char*)&obj.compression_flag, sizeof(obj.compression_flag));

    return fout;
}