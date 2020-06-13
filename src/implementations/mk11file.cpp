#include "mk11file.h"

void MK11File::read(std::ifstream& fin)
{
    unsigned int read_size = sizeof(info);
    char* read_array;

    read_array = new char [read_size];
    fin.read(read_array, read_size);    

    memcpy(&info, read_array, sizeof(info));
    delete [] read_array;

    // info.package_flags = swap_uint32(info.package_flags); //Unkown yet

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
    if (info.file_version != version_info.file_version)
    {
        throw std::string("File Version Mismatch");
    }
    if (info.licensee_version != version_info.licensee_version)
    {
        throw std::string("Licensee Mismatch");
    }
    // if (info.version_3 != version_info.version_3)
    // {
    //     throw std::string("Version 3 Mismatch");
    // }
}

void MK11File::print()
{
    std::cout<<*this<<std::endl;
}

std::ostream &operator<<(std::ostream& cout, MK11File obj)
{
    cout<<"File Info: "<<std::endl;
    cout<<"\tMagic: "<<std::hex<<obj.info.magic<<std::endl;
    cout<<"\tVersion (file/licensee): "<<std::hex<<obj.info.file_version<<"/"<<obj.info.licensee_version<<std::endl;
    cout<<"\tDecompressed Data Start Location: "<<std::hex<<obj.info.decompressed_start<<std::endl;
    cout<<"\tVersion UNK1: "<<std::hex<<obj.info.version_2<<std::endl;
    cout<<"\tEngine Version: "<<std::hex<<obj.info.engine_version<<std::endl;

    cout<<"\tGame: "; // <<std::hex<<obj.info.game_name<<std::endl;
    for (int i = 0; i < sizeof(obj.info.game_name); i++)
        cout<<obj.info.game_name[i];
    cout<<std::endl;

    cout<<"\tVersion UNK2: "<<std::hex<<obj.info.version_3<<std::endl;
    cout<<"\tVersion UNK3: "<<std::hex<<obj.info.version_4<<std::endl;
    cout<<"\tPackage Flags: "<<std::hex<<obj.info.package_flags<<" ( ";

    if (obj.info.package_flags & obj.object_flags::RF_NoFlags)
        cout<<"RF_NoFlags ";
    if (obj.info.package_flags & obj.object_flags::RF_Public)
        cout<<"RF_Public ";
    if (obj.info.package_flags & obj.object_flags::RF_Standalone)
        cout<<"RF_Standalone ";
    if (obj.info.package_flags & obj.object_flags::RF_MarkAsNative)
        cout<<"RF_MarkAsNative ";
    if (obj.info.package_flags & obj.object_flags::RF_Transactional)
        cout<<"RF_Transactional ";
    if (obj.info.package_flags & obj.object_flags::RF_ClassDefaultObject)
        cout<<"RF_ClassDefaultObject ";
    if (obj.info.package_flags & obj.object_flags::RF_ArchetypeObject)
        cout<<"RF_ArchetypeObject ";
    if (obj.info.package_flags & obj.object_flags::RF_Transient)
        cout<<"RF_Transient ";
    if (obj.info.package_flags & obj.object_flags::RF_MarkAsRootSet)
        cout<<"RF_RF_MarkAsRootSetNoFlags ";
    if (obj.info.package_flags & obj.object_flags::RF_TagGarbageTemp)
        cout<<"RF_TagGarbageTemp ";
    if (obj.info.package_flags & obj.object_flags::RF_NeedInitialization)
        cout<<"RF_NeedInitialization ";
    if (obj.info.package_flags & obj.object_flags::RF_NeedLoad)
        cout<<"RF_NeedLoad ";
    if (obj.info.package_flags & obj.object_flags::RF_KeepForCooker)
        cout<<"RF_KeepForCooker ";
    if (obj.info.package_flags & obj.object_flags::RF_NeedPostLoad)
        cout<<"RF_NeedPostLoad ";
    if (obj.info.package_flags & obj.object_flags::RF_NeedPostLoadSubobjects)
        cout<<"RF_NeedPostLoadSubobjects ";
    if (obj.info.package_flags & obj.object_flags::RF_NewerVersionExists)
        cout<<"RF_NewerVersionExists ";
    if (obj.info.package_flags & obj.object_flags::RF_BeginDestroyed)
        cout<<"RF_BeginDestroyed ";
    if (obj.info.package_flags & obj.object_flags::RF_FinishDestroyed)
        cout<<"RF_FinishDestroyed ";
    if (obj.info.package_flags & obj.object_flags::RF_BeingRegenerated)
        cout<<"RF_BeingRegenerated ";
    if (obj.info.package_flags & obj.object_flags::RF_DefaultSubObject)
        cout<<"RF_DefaultSubObject ";
    if (obj.info.package_flags & obj.object_flags::RF_WasLoaded)
        cout<<"RF_WasLoaded ";
    if (obj.info.package_flags & obj.object_flags::RF_TextExportTransient)
        cout<<"RF_TextExportTransient ";
    if (obj.info.package_flags & obj.object_flags::RF_LoadCompleted)
        cout<<"RF_LoadCompleted ";
    if (obj.info.package_flags & obj.object_flags::RF_InheritableComponentTemplate)
        cout<<"RF_InheritableComponentTemplate ";
    if (obj.info.package_flags & obj.object_flags::RF_DuplicateTransient)
        cout<<"RF_DuplicateTransient ";
    if (obj.info.package_flags & obj.object_flags::RF_StrongRefOnFrame)
        cout<<"RF_StrongRefOnFrame ";
    if (obj.info.package_flags & obj.object_flags::RF_NonPIEDuplicateTransient)
        cout<<"RF_NonPIEDuplicateTransient ";
    if (obj.info.package_flags & obj.object_flags::RF_Dynamic)
        cout<<"RF_Dynamic ";
    if (obj.info.package_flags & obj.object_flags::RF_WillBeLoaded)
        cout<<"RF_WillBeLoaded ";

    // if (obj.info.package_flags & obj.package_flags::PKG_NewlyCreated)
    //     cout<<"PKG_NewlyCreated ";
    // if (obj.info.package_flags & obj.package_flags::PKG_ClientOptional)
    //     cout<<"PKG_ClientOptional ";
    // if (obj.info.package_flags & obj.package_flags::PKG_ServerSideOnly)
    //     cout<<"PKG_ServerSideOnly ";
    // if (obj.info.package_flags & obj.package_flags::PKG_CompiledIn)
    //     cout<<"PKG_CompiledIn ";
    // if (obj.info.package_flags & obj.package_flags::PKG_ForDiffing)
    //     cout<<"PKG_ForDiffing ";
    // if (obj.info.package_flags & obj.package_flags::PKG_EditorOnly)
    //     cout<<"PKG_EditorOnly ";
    // if (obj.info.package_flags & obj.package_flags::PKG_Developer)
    //     cout<<"PKG_Developer ";
    // if (obj.info.package_flags & obj.package_flags::PKG_UncookedOnly)
    //     cout<<"PKG_UncookedOnly ";
    // if (obj.info.package_flags & obj.package_flags::PKG_ContainsMapData)
    //     cout<<"PKG_ContainsMapData ";
    // if (obj.info.package_flags & obj.package_flags::PKG_Compiling)
    //     cout<<"PKG_Compiling ";
    // if (obj.info.package_flags & obj.package_flags::PKG_ContainsMap)
    //     cout<<"PKG_ContainsMap ";
    // if (obj.info.package_flags & obj.package_flags::PKG_RequiresLocalizationGather)
    //     cout<<"PKG_RequiresLocalizationGather ";
    // if (obj.info.package_flags & obj.package_flags::PKG_PlayInEditor)
    //     cout<<"PKG_PlayInEditor ";
    // if (obj.info.package_flags & obj.package_flags::PKG_ContainsScript)
    //     cout<<"PKG_ContainsScript ";
    // if (obj.info.package_flags & obj.package_flags::PKG_DisallowExport)
    //     cout<<"PKG_DisallowExport ";
    // if (obj.info.package_flags & obj.package_flags::PKG_ReloadingForCooker)
    //     cout<<"PKG_ReloadingForCooker ";
    // if (obj.info.package_flags & obj.package_flags::PKG_FilterEditorOnly)
    //     cout<<"PKG_FilterEditorOnly ";

    cout<<")"<<std::endl;

    cout<<"\tTable Data:"<<std::endl;
    cout<<"\t\tName Table Entries: "<<std::hex<<obj.info.name_table_entries<<std::endl;
    cout<<"\t\tName Table Decompressed Offset: "<<std::hex<<obj.info.decompressed_header_location<<std::endl;
    cout<<"\t\tExport Table Entries: "<<std::hex<<obj.info.export_table_entries<<std::endl;
    cout<<"\t\tExport Table Decompressed Offset: "<<std::hex<<obj.info.decompressed_export_table_location<<std::endl;
    cout<<"\t\tImport Table Entries: "<<std::hex<<obj.info.import_table_entries<<std::endl;
    cout<<"\t\tImport Table Decompressed Offset: "<<std::hex<<obj.info.decompressed_import_table_location<<std::endl;

    cout<<"\tTotal Decompressed Size: "<<std::hex<<obj.info.decompressed_total_size<<std::endl;
    cout<<"\tFile GUID: ";
    for (uint8_t i = 0; i < sizeof(obj.info.file_GUID); i++)
    {
        cout<<std::hex<<std::setfill('0')<<std::setw(2)<<(uint16_t)obj.info.file_GUID[i]<<" ";
    }
    cout<<std::endl;

    cout<<"\tCompression Flag: "<<std::hex<<obj.info.compression_flag<<" ( ";
    if (obj.info.compression_flag & obj.compression_flags::ZLIB)
        cout<<"ZLIB ";
    if (obj.info.compression_flag & obj.compression_flags::LZO)
        cout<<"LZO ";
    if (obj.info.compression_flag & obj.compression_flags::LZX)
        cout<<"LZX ";
    if (obj.info.compression_flag & obj.compression_flags::OODLE)
        cout<<"OODLE ";
    cout<<")"<<std::endl;

    cout<<"\tPackage: "; // <<std::hex<<obj.info.main_package_name<<std::endl;
    for (int i = 0; i < sizeof(obj.info.main_package_name); i++)
        cout<<obj.info.main_package_name[i];
    cout<<std::endl;


    cout<<"\tPackages Count: "<<std::hex<<obj.info.number_of_packages<<std::endl;
    cout<<"\tExtra Packages Count: "<<std::hex<<obj.number_of_extra_packages<<std::endl;
    cout<<"\tInternal File Name: "<<std::hex<<obj.internal_file_name;
    
    return cout;
}