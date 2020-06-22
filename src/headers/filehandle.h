#ifndef FILEHANDLE_H
#define FILEHANDLE_H

#include <iostream>
#include <fstream>

class FileHandle{
    public:
        std::string file_in_name;
        std::string file_in_psf_name;
        std::string file_out_upk_name;
        std::string file_out_info_name;
        std::string file_out_table_name;
        std::string folder_out_name;
        std::string folder_out_extra_name;
        std::string output_folder = "output";
        std::string extra_folder_name = "PSFData";
        std::string extracted_folder = "extracted";
        std::string package_info_name = ".info";
        std::string name_table_name = ".names_table";
        std::string export_table_name = ".exports_table";
        std::string import_table_name = ".imports_table";
        std::string bulks_table_name = ".bulkdata_table";
        std::string additional_table_name = ".additionaldata_table";
        std::string testing_files[4] = {
            "BARAKA_A_CHARINTRO_ScriptAssets.XXX", // XXX File without PSF
            "GEARASSETS_KIT_ScriptAssets.XXX", // XXX File with PSF
            "Init.XXX", /// XXX file with 1 of each segments
            "TweakVars.xxx" /// XXX file with Zlib Decompression
        };
        struct Extensions {
            std::string package = ".xxx";
            std::string extra_package = ".psf";
            std::string compressed_file = ".oodle";
            std::string decompressed_file = ".dec";
            std::string unpacked_file = ".upk";
            std::string table_file = ".txt";
        } extensions;

    std::ifstream file_in;
    std::ifstream file_in_psf;
    std::ifstream file_in_upk;
    std::ofstream file_out_upk;
    // std::ofstream file_out_info;
    // std::ofstream file_out_table;

    FileHandle() {};
    FileHandle(std::string);
    void print();
    void open_files();
    void open_files_out();
    void set(std::string);
    void swap_upk();
    std::string get_psf_name(std::string);
    std::string get_base_dir(std::string);
    std::string get_base_name(std::string);
    std::string join(std::string*, uint64_t);
    std::string get_name_no_extension(std::string);
    std::string make_file_out_name(uint64_t, uint64_t);
    std::string make_folder_out_name(uint64_t, std::string);
    std::string reext(std::string, std::string);
    void mkdirs(std::string);
    void swap_table_name();
    void swap_table_import();
    void swap_table_export();
    void swap_table_bulk();
    void swap_table_additional();
    friend std::ostream& operator<<(std::ostream&, FileHandle&);
};

#endif