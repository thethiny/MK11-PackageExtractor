#include "mk11file.h"

void MK11File::read(std::ifstream& fin)
{
    unsigned int read_size = sizeof(info);
    char* read_array;

    read_array = new char [read_size];
    fin.read(read_array, read_size);    

    memcpy(&info, read_array, sizeof(info));

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
    if (!input_file_obj->file_in_psf)
    {
        std::cerr<<"PSF file couldn't be opened. Skipping.";
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
    cout<<"Magic: "<<std::hex<<obj.info.magic<<std::endl;
    cout<<"Version: "<<std::hex<<obj.info.version_1<<"/"<<obj.info.version_2<<std::endl;
    cout<<"UNK1: "<<std::hex<<obj.info.unk1<<std::endl;
    cout<<"UNK2: "<<std::hex<<obj.info.unk2<<std::endl;
    cout<<"EngineVer: "<<std::hex<<obj.info.engine_version<<std::endl;
    cout<<"Game: "<<std::hex<<obj.info.game_name<<std::endl;
    cout<<"ScriptVer: "<<std::hex<<obj.info.version_3<<std::endl;
    cout<<"UNK3: "<<std::hex<<obj.info.unk3<<std::endl;
    cout<<"Package: "<<std::hex<<obj.info.main_package_name<<std::endl;
    cout<<"Packages Count: "<<std::hex<<obj.info.number_of_packages<<std::endl;
    cout<<"Extra Packages Count: "<<std::hex<<obj.number_of_extra_packages<<std::endl;
    cout<<"Internal File Name: "<<std::hex<<obj.internal_file_name;
    
    return cout;
}