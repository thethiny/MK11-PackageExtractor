#include "filehandle.h"

FileHandle::FileHandle(std::string fname)
{
    set(fname);
}

void FileHandle::set(std::string fname)
{
    file_in_name = fname;
    file_in_psf_name = fname.substr(0, fname.length()-4) + ".psf";
}

void FileHandle::print()
{
    std::cout<<"File Name: "<<file_in_name<<std::endl;
    std::cout<<"PSF File Name: "<<file_in_psf_name<<std::endl;
}

std::ostream& operator<<(std::ostream& cerr, FileHandle obj)
{
    cerr<<"File Name: "<<obj.file_in_name<<std::endl;
    cerr<<"PSF File Name: "<<obj.file_in_psf_name;
    return cerr;
}

std::string FileHandle::get_file_path(std::string fname)
{
    return (files_path + fname);
}

std::string FileHandle::get_file_path()
{
    return (files_path + file_in_name);
}