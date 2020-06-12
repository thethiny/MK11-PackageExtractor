#include "zlib.h"

void Zlib::resolve_functions()
{
    zlib_compress = (CompressFunc*)GetProcAddress(dll_object, compress_function_name.c_str());
    if (!zlib_compress)
    {
        throw std::string("Couldn't load ") + compress_function_name + std::string(" from dll");
    }

    zlib_uncompress = (UncompressFunc*)GetProcAddress(dll_object, decompress_function_name.c_str());
    if (!zlib_uncompress)
    {
        throw std::string("Couldn't load ") + decompress_function_name + std::string(" from dll");
    }

    zlib_compress_bound = (CompressBoundFunc*)GetProcAddress(dll_object, compress_helper_function.c_str());
    if (!zlib_compress_bound)
    {
        throw std::string("Couldn't load ") + compress_function_name + std::string(" from dll");
    }

}

int64_t Zlib::Compress(char *src_buf, uint64_t src_len, char* dst_buf) //Untested Function
{
    uint64_t dst_len = zlib_compress_bound(src_len);
    zlib_compress(dst_buf, &dst_len, src_buf, src_len, compression_level);
    return dst_len;
}

void Zlib::Decompress(char* src_buf, int64_t src_len, char* dst_buf, uint64_t dst_len)
{
    int32_t error_level;

    error_level = zlib_uncompress(dst_buf, &dst_len, src_buf, src_len);
    if (error_level)
    {
        throw std::string("Decompression Error");
    }
}