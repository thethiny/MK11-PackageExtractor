#include "compressedsegment.h"

void CompressedSegment::read(std::ifstream& fin)
{
    uint32_t read_size = sizeof(info);
    char* read_array = new char [read_size];
    fin.read(read_array, read_size);
    memcpy(&info, read_array, read_size);
    delete [] read_array;
}

void CompressedSegment::read_data(std::ifstream& fin)
{
    fin.seekg(data_location);
    data = new char [info.compressed_segment_compressed_size];
    fin.read(data, info.compressed_segment_compressed_size);
}

std::ostream& operator<<(std::ostream& cout, CompressedSegment obj)
{
    cout<<"\t\tCompressed Segment #"<<obj.id<<": "<<std::endl;
    cout<<"\t\t\tCompressed Size: "<<obj.info.compressed_segment_compressed_size<<std::endl;
    cout<<"\t\t\tDecompressed Size: "<<obj.info.compressed_segment_decompressed_size<<std::endl;
    cout<<"\t\t\tData Location: "<<obj.data_location;

    return cout;
}

std::ofstream& operator<<(std::ofstream& fout, CompressedSegment obj)
{
    uint8_t* dec_size_arr = new uint8_t [8];
    btoa(dec_size_arr, obj.info.compressed_segment_decompressed_size, 8);
    for (uint8_t i = 0; i < 8; i++)
    {
        fout<<dec_size_arr[i];
    }

    for (uint64_t i = 0; i < obj.info.compressed_segment_compressed_size; i++)
    {
        fout<<(uint8_t)(obj.data[i]);
    }

    return fout;
}