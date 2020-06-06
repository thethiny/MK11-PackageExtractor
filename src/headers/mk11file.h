#ifndef MK11HEADER_H
#define MK11HEADER_H

#include "package.h"
#include "filehandle.h"

class MK11File{
    private:
        struct VersionInfo{
            unsigned int magic = 0x9E2A83C1u;
            unsigned int engine_version = 0x01E7u;
            unsigned int version_3 = 0x50;
            unsigned short version_1 = 0x0301u;
            unsigned short version_2 = 0x9Du;
        } version_info;
        void validate_header();

    public:
        struct Info{
            uint32_t magic;
            uint16_t version_1;
            uint16_t version_2;
            uint32_t unk1;
            uint32_t unk2;
            uint32_t engine_version;
            char game_name[4];
            uint32_t version_3;
            uint32_t unk3;
            char main_package_name[4];
            char unk_large[0x44];
            uint32_t number_of_packages;
        } __attribute__((packed)) info; 

        uint32_t number_of_extra_packages;
        char* internal_file_name;
        bool has_psf;
        bool load_psf = true;

        Package* packages;
        Package* packages_extra;

        FileHandle* input_file_obj;


        void read(std::ifstream&);
        void read_footing(std::ifstream&);
        void read_extra_tables(std::ifstream&);
        void read_packages(std::ifstream&);
        void read_packages_extra(std::ifstream&);
        void read_compressed_segments(std::ifstream&);
        void read_compressed_segments_extra();
        void register_file(FileHandle& file) {input_file_obj = &file;}
        void set_psf_status(bool status) {has_psf = status && number_of_extra_packages && load_psf;}
        bool get_psf_status() {return has_psf;}
        void print();
        friend std::ostream &operator<<(std::ostream&, MK11File);

};

#endif