#ifndef EXTRA_TABLE_H
#define EXTRA_TABLE_H

#include "extra_table_entry.h"

class ExtraTable{
    public:
        struct {
            uint32_t unk1;
            uint32_t unk2;
            uint32_t name_len;
        } __attribute__((packed)) header;
        char* name;
        uint32_t entries_count;
        uint32_t compression_flag;
        ExtraTableEntry* entries;
        uint32_t id;

    void read(std::ifstream&);
    friend std::ostream& operator<<(std::ostream&, ExtraTable);
    friend std::ofstream& operator<<(std::ofstream&, ExtraTable);
};

#endif