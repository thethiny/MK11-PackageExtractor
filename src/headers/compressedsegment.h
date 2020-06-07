#ifndef COMPRESSEDSEGMENT_H
#define COMPRESSEDSEGMENT_H

#include <iostream>
#include <fstream>
#include <windows.h>
#include "utils.h"

class CompressedSegment{
    public:
        struct Info{
            uint64_t compressed_segment_compressed_size;
            uint64_t compressed_segment_decompressed_size;
        } __attribute__((packed)) info;
        char* data;
        unsigned int data_location;
        unsigned int id;

        void read(std::ifstream&);
        friend std::ostream& operator<<(std::ostream&, CompressedSegment);
        friend std::ofstream& operator<<(std::ofstream&, CompressedSegment);
        void read_data(std::ifstream&);
        void delete_data() {delete [] data;}
};

#endif