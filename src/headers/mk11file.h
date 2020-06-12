#ifndef MK11HEADER_H
#define MK11HEADER_H

#include "package.h"
#include "filehandle.h"
#include "iomanip"

class MK11File{
    private:
        struct VersionInfo{
            unsigned int magic = 0x9E2A83C1u;
            unsigned int engine_version = 0x01E7u;
            unsigned int version_3 = 0x50;
            unsigned short file_version = 0x0301u;
            unsigned short licensee_version = 0x9Du;
        } version_info;
        void validate_header();

    public:
        struct Info{
            uint32_t magic;
            uint16_t file_version;
            uint16_t licensee_version;
            uint32_t decompressed_start; // Location of Data in Total Decompressed Data
            uint32_t version_2; // Maybe
            uint32_t engine_version;
            char game_name[4];
            uint32_t version_3;
            uint32_t version_4;
            char main_package_name[4];
            uint32_t package_flags;
            uint32_t name_table_entries;
            uint64_t decompressed_header_location; // Location of Header in the Total Decompressed Data
            uint32_t export_table_entries;
            uint64_t decompressed_export_table_location;
            uint32_t import_table_entries;
            uint64_t decompressed_import_table_location;
            uint64_t decompressed_total_size; // Without files that have _BulkData suffix, need to figure out how to determine bulkdata, prob flag in expTbl.
            uint8_t file_GUID[0x10];
            uint32_t compression_flag; // Most Probably
            uint32_t number_of_packages;
        } __attribute__((packed)) info; 

        enum compression_flags
        {
            ZLIB  = 0x0001u,
            LZO   = 0x0002u,
            LZX   = 0x0004u,
            OODLE = 0x0100u
        };

        uint32_t number_of_extra_packages = 0;
        char* internal_file_name;
        bool has_psf;
        bool load_psf = true;

        Package* packages;
        Package* packages_extra;

        const uint64_t MAX_DEC_SIZE = 0x20000; // Maximum Size for a chunk of uncompressed data

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