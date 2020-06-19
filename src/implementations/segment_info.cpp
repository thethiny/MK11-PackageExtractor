#include "segment_info.h"

struct Chunk {
    uint64_t compressed_size;
    uint64_t decompressed_size;
} __attribute__((packed));

void SegmentInfo::read(std::ifstream& fin)
{
    compressed_segments_count = 0;
    uint64_t dec_sum = 0;
    std::streampos cur_position = fin.tellg();

    uint32_t read_size = sizeof(info);
    char* read_array = new char [read_size];
    fin.read(read_array, read_size);
    memcpy(&info, read_array, read_size);
    delete [] read_array;

    while (dec_sum < info.segment_decompressed_size) // Replace this with precalculated segment count (ceil(dec_size/max_size))
    {
        Chunk chunk;
        uint64_t read_size = sizeof(chunk);
        char* read_array = new char [read_size];
        fin.read(read_array, read_size);
        memcpy(&chunk, read_array, read_size);
        dec_sum += chunk.decompressed_size;
        compressed_segments_count++;
        delete [] read_array;
    }

    uint64_t data_location = fin.tellg();
    fin.seekg(cur_position);
    fin.seekg(0x20, std::ios::cur); //Skip 0x20 to go directly to sizes

    compressed_segments = new CompressedSegment [compressed_segments_count];
    for (uint64_t i = 0; i < compressed_segments_count; i++)
    {
        compressed_segments[i].read(fin);
        compressed_segments[i].id = i;
        compressed_segments[i].data_location = data_location;
        data_location += compressed_segments[i].info.compressed_segment_compressed_size;
    }

    // Skip to the next block
    fin.seekg(info.segment_compressed_size, std::ios::cur);
    // Alternate
    // fin.seekg(data_location)
    // Alternate
    // Before calling fn: fin.seekg(info.st_location)
    
}

std::ostream &operator<<(std::ostream& cout, SegmentInfo obj)
{
    cout<<"\t\tInfo:"<<std::endl;
    cout<<"\t\tUnknown: "<<obj.info.unk<<std::endl;
    cout<<"\t\tMaximum Segment Decompressed Size: "<<obj.info.max_seg_dec_size<<std::endl;
    cout<<"\t\tFull Compressed Size: "<<obj.info.segment_compressed_size<<std::endl;
    cout<<"\t\tFull Decompressed Size: "<<obj.info.segment_decompressed_size<<std::endl;
    cout<<"\t\tCompressed Segments Found: "<<obj.compressed_segments_count;

    return cout;
}