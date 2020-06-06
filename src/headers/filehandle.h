#ifndef FILEHANDLE_H
#define FILEHANDLE_H

#include <iostream>

class FileHandle{
    public:
        std::string file_in_name;
        std::string file_out_name;
        std::string file_in_psf_name;
        std::string files_path = "files\\";
        std::string testing_files[3] = {
            "BARAKA_A_CHARINTRO_ScriptAssets.XXX",
            "GEARASSETS_KIT_ScriptAssets.XXX",
            "Init.XXX"
        };

    FileHandle(std::string);

    void set(std::string);
    void print();
    std::string get_file_path();
    std::string get_file_path(std::string);
    friend std::ostream& operator<<(std::ostream&, FileHandle);


};

#endif