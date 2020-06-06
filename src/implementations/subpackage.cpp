#include "subpackage.h"

void SubPackage::read(std::ifstream& fin)
{
    unsigned int read_size = sizeof(info);
    char* read_array = new char [read_size];
    fin.read(read_array, read_size);

    memcpy(&info, read_array, read_size);
}

void SubPackage::read_info(std::ifstream& fin)
{
    segment.id = id;
    segment.read(fin);
}

void SubPackage::read_extra_info(std::string psf_fname)
{
    // Should be same as above function fin changes
    std::ifstream fin(psf_fname.c_str(), std::ios::binary);
    fin>>std::noskipws;
    read_info(fin);
}

std::ostream &operator<<(std::ostream& cout, SubPackage obj)
{
    cout<<"\tSubPackage #"<<obj.id<<":"<<std::endl;
    cout<<"\t\tUnknown: "<<obj.info.unkown<<std::endl;
    cout<<"\t\tDecompressed Size: "<<obj.info.dec_size<<std::endl;
    cout<<"\t\tStart Offset: "<<obj.info.st_offset<<std::endl;
    cout<<"\t\tSegment Size: "<<obj.info.seg_size;

    return cout;
}