#ifndef PACKAGE_H
#define PACKAGE_H

#include "subpackage.h"

class Package{
    public:
        uint32_t name_len;
        char* name;
        struct Info{
            uint64_t unk;
            uint64_t decompressed_size;
            uint64_t start_offset;
            uint64_t segment_size;
            uint32_t number_of_subpackages; // For some reason this is treated as 8 not 4
        } __attribute__((packed)) info;
        unsigned int id;
        SubPackage* subpackages;

        void read(std::ifstream&);
        friend std::ostream &operator<<(std::ostream&, Package);
};


#endif