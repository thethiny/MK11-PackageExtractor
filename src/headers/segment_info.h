#ifndef SEGMENT_INFO_H
#define SEGMENT_INFO_H

#include "compressedsegment.h"

class SegmentInfo{
    public:
        struct Info{
            uint32_t magic;
            uint32_t unk;
            uint64_t max_seg_dec_size;
            uint64_t segment_compressed_size;
            uint64_t segment_decompressed_size;
        } __attribute__((packed)) info;

        unsigned int compressed_segments_count = 0;
        unsigned int id;
        CompressedSegment* compressed_segments;

        void read(std::ifstream&);
        friend std::ostream &operator<<(std::ostream&, SegmentInfo);
};

#endif