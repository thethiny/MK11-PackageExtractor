#include "utils.h"

void ArgumentParser::set(int argv, const char** argc)
{
    std::string value;
    this->argv = argv;
    this->argc = argc;
    if (!validate())
    {
        show_help();
    }

    value = get_value("-f");
    if (value == "")
    {
        show_help();
    }
    set_file_name(value);

    value = get_value("-p");
    if (value == "")
    {
        set_load_psf_flag(true); //Default Value
    }
    else
    {
        set_load_psf_flag(bool(std::stoi(value)));
    }

    value = get_value("-d");
    if (value == "")
    {
        set_dll_folder("");
    }
    else
    {
        set_dll_folder(value);
    }
    
}

void ArgumentParser::set_file_name(std::string file_name)
{
    this->file_name = file_name;
}

void ArgumentParser::set_dll_folder(std::string dll_folder)
{
    this->dll_folder = dll_folder;
}

void ArgumentParser::set_load_psf_flag(bool flag)
{
    this->load_psf = flag;
}

std::string ArgumentParser::get_value(std::string flag)
{
    for (int i = 1; i < argv; i++)
    {
        if (std::string(argc[i]) == flag)
        {
            if (i+1 < argv)
            {
                return std::string(argc[i+1]);
            }
            else
            {
                show_help();
            }
            
        }
    }
    return std::string("");
}

void ArgumentParser::show_help()
{
    std::cerr<<"Usage:\n\t";
    std::cerr<<argc[0]<<" [-options]\n";
    std::cerr<<"\tOptions:\n\t\t-f file_name [-p] load_psf [-d] dll_folder\n";
    std::cerr<<"\tMandatory:\n\t\t-f"<<std::endl;
    throw std::string("Incorrect Usage");
}

uint32_t swap_uint32(uint32_t to_swap)
{
    uint32_t swapped = 0;
    for (int i = 0; i < 4; i++)
    {
        swapped <<= 8;
        swapped |= (uint8_t)to_swap;
        to_swap >>= 8;
    }
    return swapped;
}