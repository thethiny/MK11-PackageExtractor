#ifndef SUBPACKAGE_H
#define SUBPACKAGE_H

#include "segment_info.h"

class SubPackage{
    public:
        struct Info{
            uint64_t unkown;
            uint64_t dec_size;
            uint64_t st_offset;
            uint64_t seg_size;
        } __attribute__((packed)) info;
        unsigned int id;
        //uint32_t number_of_segments = -1; //-1 Not Implemented Yet
        SegmentInfo segment;

        void read(std::ifstream&);
        void read_info(std::ifstream&);
        void read_extra_info(std::string); // Read info for extra packages, requires psf file name
        friend std::ostream &operator<<(std::ostream&, SubPackage);
};

#endif