#include "filehandle.h"
#include "direct.h"
#include <sstream>

FileHandle::FileHandle(std::string fname)
{
    set(fname);
    open_files();
}

void FileHandle::set(std::string fname)
{
    file_in_name = fname;
    file_in_psf_name = get_psf_name(fname);
    file_out_upk_name = reext(get_base_name(fname), extensions.unpacked_file);
    std::string params[2] = {output_folder, get_base_name(fname)};
    folder_out_name = join(params, 2);
    std::string params2[2] = {folder_out_name, extra_folder_name};
    folder_out_extra_name = join(params2, 2);
    params2[1] = file_out_upk_name;
    file_out_upk_name = join(params2, 2);
}

void FileHandle::swap_upk()
{
    file_out_upk.close();
    file_in_upk.open(file_out_upk_name, std::ios::binary);
    if (!file_in_upk.good())
    {
        throw std::string("Error Opening UPK file for reading.");
    }
}

std::string FileHandle::join(std::string* names, uint64_t count)
{
    std::string full_string = "";
    for (uint64_t i = 0; i < count; i++)
    {
        full_string += names[i];
        if (i + 1 < count)
        {
            full_string += '\\';
        }
    }
    return full_string;
}

std::string FileHandle::get_base_dir(std::string file_name)
{
    std::string file_no_ext = "";
    uint16_t index = 0;
    bool f = false;
    for (uint16_t i = 0; i < file_name.length(); i++)
    {
        if (file_name[i] == '\\')
        {
            index = i;
            f = true;
        }
    }
    if (f)
        file_no_ext = file_name.substr(index+1, file_name.length());
    else
        file_no_ext = file_name;

    return file_no_ext;
}

std::string FileHandle::get_name_no_extension(std::string file_name)
{
    std::string base_name = "";

    uint16_t index = 0;
    bool f = false;
    for (uint16_t i = 0; i < file_name.length(); i++)
    {
        if (file_name[i] == '.')
        {
            index = i;
            f = true;
        }
    }
    if (f)
        base_name = file_name.substr(0, index);
    else base_name = file_name;

    return base_name;
}

std::string FileHandle::get_base_name(std::string file_name)
{
    std::string base_name = get_base_dir(file_name);
    std::string file_no_ext = get_name_no_extension(base_name);

    return file_no_ext;
}

std::string FileHandle::get_psf_name(std::string file_name)
{
    return get_name_no_extension(file_name) + extensions.extra_package;
}

std::string FileHandle::reext(std::string file_name, std::string extension)
{
    return get_name_no_extension(file_name) + extension;
}

void FileHandle::print()
{
    std::cout<<"File Name: "<<file_in_name<<std::endl;
    std::cout<<"PSF File Name: "<<file_in_psf_name<<std::endl;
    std::cout<<"UPK File Name: "<<file_out_upk_name<<std::endl;
}

std::ostream& operator<<(std::ostream& cout, FileHandle &obj)
{
    cout<<"File Name: "<<obj.file_in_name<<std::endl;
    cout<<"PSF File Name: "<<obj.file_in_psf_name<<std::endl;
    cout<<"UPK File Name: "<<obj.file_out_upk_name<<std::endl;
    cout<<"Output Name: "<<obj.folder_out_name;
    return cout;
}

void FileHandle::open_files()
{
    file_in.open(file_in_name, std::ios::binary);
    if (!file_in)
    {
        throw std::string("Couldn't open " + file_in_name);
    }
    file_in>>std::noskipws;

    file_in_psf.open(file_in_psf_name, std::ios::binary);
    if (!file_in_psf)
    {
        std::cerr<<"PSF File not present."<<std::endl;
    }
    file_in_psf>>std::noskipws;
    
}

void FileHandle::open_files_out()
{
    file_out_upk.open(file_out_upk_name, std::ios::binary);
    if (!file_out_upk)
    {
        throw std::string("Couldn't make output file " + file_out_upk_name);
    }
}

std::string FileHandle::make_file_out_name(uint64_t seg_id, uint64_t chunk_id)
{
    std::stringstream file_name("");
    file_name<<seg_id<<"_"<<chunk_id<<extensions.compressed_file;

    return file_name.str();
}

std::string FileHandle::make_folder_out_name(uint64_t pack_id, std::string pack_name)
{
    std::stringstream folder_name("");
    folder_name<<pack_id<<"_"<<pack_name;

    return folder_name.str();
}