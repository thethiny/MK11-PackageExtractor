#include "compressedsegment.h"

void CompressedSegment::read(std::ifstream& fin)
{
    uint32_t read_size = sizeof(info);
    char* read_array = new char [read_size];
    fin.read(read_array, read_size);
    memcpy(&info, read_array, read_size);
}

std::ostream& operator<<(std::ostream& cout, CompressedSegment obj)
{
    cout<<"\t\tCompressed Segment #"<<obj.id<<": "<<std::endl;
    cout<<"\t\t\tCompressed Size: "<<obj.info.compressed_segment_compressed_size<<std::endl;
    cout<<"\t\t\tDecompressed Size: "<<obj.info.compressed_segment_decompressed_size;

    return cout;
}