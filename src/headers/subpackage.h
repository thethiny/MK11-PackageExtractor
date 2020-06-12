#ifndef SUBPACKAGE_H
#define SUBPACKAGE_H

#include "segment_info.h"

class SubPackage{
    public:
        struct Info{
            uint64_t decompressed_offset;
            uint64_t dec_size;
            uint64_t st_offset;
            uint64_t seg_size;
        } __attribute__((packed)) info;
        unsigned int id;
        SegmentInfo segment;

        void read(std::ifstream&);
        void read_info(std::ifstream&);
        friend std::ostream &operator<<(std::ostream&, SubPackage);
};

#endif