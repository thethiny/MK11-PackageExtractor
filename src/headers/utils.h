#ifndef UTILS_H
#define UTILS_H
#include <iostream>

template <class dtype>
void btoa(uint8_t* char_array, dtype value, uint64_t size)
{
    uint64_t mask = 0xFF;
    for (uint64_t i = 0; i < size; i++)
    {
        char_array[i] = (mask & value)>>(8*i);
        mask <<= 8;
    }

}

uint32_t swap_uint32(uint32_t);

class ArgumentParser
{
    public:
        std::string file_name;
        std::string dll_folder;
        bool load_psf;
        int argv;
        const char** argc;

        ArgumentParser(){};
        ArgumentParser(int argv, const char** argc) {this->argv = argv; this->argc = argc;}
        void set(int, const char**);
        void set_file_name(std::string);
        void set_dll_folder(std::string);
        void set_load_psf_flag(bool);
        std::string get_value(std::string);
        bool validate() {return argv >= 2;}
        void show_help();
};

#endif