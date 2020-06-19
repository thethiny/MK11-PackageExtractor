#ifndef EXTRA_TABLE_ENTRY_H
#define EXTRA_TABLE_ENTRY_H

#include <iostream>
#include <fstream>
#include <windows.h>

class ExtraTableEntry{
    public:
        struct{
            uint64_t decompressed_size;
            uint64_t compressed_size;
            uint64_t decompressed_offset;
            uint64_t compressed_offset;
        } __attribute__((packed)) info;
        uint32_t id;

    void read(std::ifstream&);
    friend std::ostream& operator<<(std::ostream&, ExtraTableEntry);
    friend std::ofstream& operator<<(std::ofstream&, ExtraTableEntry);
};

#endif