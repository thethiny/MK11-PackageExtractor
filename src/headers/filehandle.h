#ifndef FILEHANDLE_H
#define FILEHANDLE_H

#include <iostream>
#include <fstream>

class FileHandle{
    public:
        std::string file_in_name;
        std::string file_in_psf_name;
        std::string folder_out_name;
        std::string folder_out_extra_name;
        std::string output_folder = "output";
        std::string extra_folder_name = "Extra";
        std::string testing_files[3] = {
            "BARAKA_A_CHARINTRO_ScriptAssets.XXX", // XXX File without PSF
            "GEARASSETS_KIT_ScriptAssets.XXX", // XXX File with PSF
            "Init.XXX" /// XXX file with 1 of each segments
        };
        struct Extensions {
            std::string package = ".xxx";
            std::string extra_package = ".psf";
            std::string compressed_file = ".oodle";
            std::string decompressed_file = ".dec";
        } extensions;

    std::ifstream file_in;
    std::ifstream file_in_psf;

    FileHandle() {};
    FileHandle(std::string);
    void print();
    void open_files();
    void set(std::string);
    std::string get_psf_name(std::string);
    std::string get_base_dir(std::string);
    std::string get_base_name(std::string);
    std::string join(std::string*, uint64_t);
    std::string get_name_no_extension(std::string);
    std::string make_file_out_name(uint64_t, uint64_t);
    std::string make_folder_out_name(uint64_t, std::string);
    friend std::ostream& operator<<(std::ostream&, FileHandle&);


};

#endif