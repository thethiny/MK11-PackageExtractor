#include "compression.h"

void Compression::load_dll(std::string dll_path)
{
    dll_object = LoadLibraryA(dll_path.c_str());

    if (!dll_object)
    {
        throw std::string("Failed to load Oodle dll ") + dll_path;
    }

    resolve_functions();
    std::cerr<<dll_path<<" Loaded Successfully."<<std::endl;
}
