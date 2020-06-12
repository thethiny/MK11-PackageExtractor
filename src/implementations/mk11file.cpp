#include "mk11file.h"

void MK11File::read(std::ifstream& fin)
{
    unsigned int read_size = sizeof(info);
    char* read_array;

    read_array = new char [read_size];
    fin.read(read_array, read_size);    

    memcpy(&info, read_array, sizeof(info));
    delete [] read_array;

    try
    {
        validate_header();
    }
    catch (...)
    {
        throw;
    }
    

    packages = new Package[info.number_of_packages];

    read_packages(fin);
    read_packages_extra(fin);
    fin.seekg(0x18u, std::ios::cur); // 0x18 Bytes of Padding
    read_footing(fin);
    read_extra_tables(fin);
    read_compressed_segments(fin);
    read_compressed_segments_extra();

}

void MK11File::read_footing(std::ifstream& fin)
{
    char read_array[4];
    uint32_t name_len;
    fin.read(read_array, 4);
    memcpy(&name_len, read_array, 4);
    internal_file_name = new char [name_len];

    fin.read(internal_file_name, name_len);


}

void MK11File::read_extra_tables(std::ifstream& fin)
{
    /* Not Yet Implemented as lack of knowledge arises,
    however first 4 bytes indicate references to PSF and then read,
    then first 4 bytes indicate tables, then 4 bytes table entries. */
    fin.seekg(packages[0].subpackages[0].info.st_offset); // For now, just go to first entry

}

void MK11File::read_packages(std::ifstream& fin)
{
    for (uint32_t i = 0; i < info.number_of_packages; i++)
    {
        packages[i].read(fin);
        packages[i].id = i;
    }
}

void MK11File::read_packages_extra(std::ifstream& fin)
{
    char read_array[4];
    fin.read(read_array, 4);

    memcpy(&number_of_extra_packages, read_array, 4);

    packages_extra = new Package [number_of_extra_packages];

    for (uint32_t i = 0; i < number_of_extra_packages; i++)
    {
        packages_extra[i].read(fin);
        packages_extra[i].id = i + info.number_of_packages;
    }

}

void MK11File::read_compressed_segments(std::ifstream& fin)
{
    for (uint32_t i = 0; i < info.number_of_packages; i++)
    {
        for (uint32_t j = 0; j < packages[i].info.number_of_subpackages; j++)
        {
            packages[i].subpackages[j].read_info(fin);
        }
    }
}

void MK11File::read_compressed_segments_extra()
{
    if (!number_of_extra_packages)
    {
        std::cerr<<"PSF File Not Required."<<std::endl;
        return;
    }
    else if (!input_file_obj->file_in_psf)
    {
        std::cerr<<"Couldn't find required PSF File... Skipping."<<std::endl;
        return;
    }
    set_psf_status(true);

    for (uint32_t i = 0; i < number_of_extra_packages; i++)
    {
        for (uint32_t j = 0; j < packages_extra[i].info.number_of_subpackages; j++)
        {
            packages_extra[i].subpackages[j].read_info(input_file_obj->file_in_psf);
        }
    }
}

void MK11File::validate_header()
{
    if (info.magic != version_info.magic)
    {
        throw std::string("Magic Mismatch");
    }
    if (info.engine_version != version_info.engine_version)
    {
        throw std::string("Engine Version Mismatch");
    }
    if (info.version_1 != version_info.version_1)
    {
        throw std::string("Version 1 Mismatch");
    }
    if (info.version_2 != version_info.version_2)
    {
        throw std::string("Version 2 Mismatch");
    }
    if (info.version_3 != version_info.version_3)
    {
        throw std::string("Version 3 Mismatch");
    }
}

void MK11File::print()
{
    std::cout<<*this<<std::endl;
}

std::ostream &operator<<(std::ostream& cout, MK11File obj)
{
    cout<<"File Info: "<<std::endl;
    cout<<"\tMagic: "<<std::hex<<obj.info.magic<<std::endl;
    cout<<"\tVersion: "<<std::hex<<obj.info.version_1<<"/"<<obj.info.version_2<<std::endl;
    cout<<"\tDecompressed Data Start Location: "<<std::hex<<obj.info.decompressed_start<<std::endl;
    cout<<"\tVersion UNK1: "<<std::hex<<obj.info.file_version<<std::endl;
    cout<<"\tEngine Version: "<<std::hex<<obj.info.engine_version<<std::endl;
    cout<<"\tGame: "<<std::hex<<obj.info.game_name<<std::endl;
    cout<<"\tVersion UNK2: "<<std::hex<<obj.info.version_3<<std::endl;
    cout<<"\tVersion UNK3: "<<std::hex<<obj.info.version_4<<std::endl;
    cout<<"\tUNK: "<<std::hex<<obj.info.unk1<<std::endl;

    cout<<"\tTable Data:"<<std::endl;
    cout<<"\t\tName Table Entries: "<<std::hex<<obj.info.name_table_entries<<std::endl;
    cout<<"\t\tName Table Decompressed Offset: "<<std::hex<<obj.info.decompressed_header_location<<std::endl;
    cout<<"\t\tTable 2 Entries: "<<std::hex<<obj.info.table_2_entries<<std::endl;
    cout<<"\t\tTable 2 Decompressed Offset: "<<std::hex<<obj.info.decompressed_table_2_location<<std::endl;
    cout<<"\t\tTable 3 Entries: "<<std::hex<<obj.info.table_3_entries<<std::endl;
    cout<<"\t\tTable 3 Decompressed Offset: "<<std::hex<<obj.info.decompressed_table_3_location<<std::endl;

    cout<<"\tTotal Decompressed Size: "<<std::hex<<obj.info.decompressed_total_size<<std::endl;
    cout<<"\tFile GUID: ";
    for (uint8_t i = 0; i < sizeof(obj.info.file_GUID); i++)
    {
        cout<<std::hex<<std::setfill('0')<<std::setw(2)<<(uint16_t)obj.info.file_GUID[i]<<" ";
    }
    cout<<std::endl;

    cout<<"\tUNK 2:"<<std::hex<<obj.info.unk2<<std::endl;

    cout<<"\tPackage: "<<std::hex<<obj.info.main_package_name<<std::endl;
    cout<<"\tPackages Count: "<<std::hex<<obj.info.number_of_packages<<std::endl;
    cout<<"\tExtra Packages Count: "<<std::hex<<obj.number_of_extra_packages<<std::endl;
    cout<<"\tInternal File Name: "<<std::hex<<obj.internal_file_name;
    
    return cout;
}