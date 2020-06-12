#include "oodle.h"

void Oodle::resolve_functions()
{
    oodl_compress = (OodleLZ_CompressFunc*)GetProcAddress(dll_object, compress_function_name.c_str());

    if (!oodl_compress)
    {
        throw std::string("Couldn't load ") + compress_function_name + std::string(" from dll");
    }

    oodl_decompress = (OodleLZ_DecompressFunc*)GetProcAddress(dll_object, decompress_function_name.c_str());

    if (!oodl_decompress)
    {
        throw std::string("Couldn't load ") + decompress_function_name + std::string(" from dll");
    }

    oodle_get_default = (OodleLZ_CompressOptions_GetDefaultFunc*)GetProcAddress(dll_object, compress_function_name.c_str());
    if (!oodle_get_default)
    {
        throw std::string("Couldn't load ") + compress_function_name + std::string(" from dll");
    }

}

int64_t Oodle::Compress(char *src_buf, uint64_t src_len, char* dst_buf)
{
    return oodl_compress(compression_type, src_buf, src_len, dst_buf, compression_level, 0, 0);
}

void Oodle::Decompress(char* src_buf, int64_t src_len, char* dst_buf, uint64_t dst_len)
{
    int data_size = oodl_decompress(src_buf, src_len, dst_buf, dst_len, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    oodl_decompress(src_buf, src_len, dst_buf, dst_len, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    if (data_size < 0)
    {
        throw std::string("Decompression Error");
    }
}