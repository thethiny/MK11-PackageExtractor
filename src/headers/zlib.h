#ifndef ZLIB_H
#define ZLIB_H

#include "compression.h"


class Zlib: public Compression{
    public:
        std::string dll_path = "./zlib1.dll";
        std::string compress_function_name = "compress2";
        std::string decompress_function_name = "uncompress";
        std::string compress_helper_function = "compressBound";

        typedef int __cdecl UncompressFunc(char* dst_buf, uint64_t* dst_len, char* src_buf, uint64_t src_len);
        typedef int __cdecl CompressFunc(char* dst_buf, uint64_t* dst_len, char* src_buf, uint64_t src_len, int32_t level);
        typedef uint64_t __cdecl CompressBoundFunc(uint64_t src_len);

        UncompressFunc* zlib_uncompress;
        CompressFunc* zlib_compress;
        CompressBoundFunc* zlib_compress_bound;

        static const int32_t compression_level = 0;
        static const int32_t compression_arg = 2;

        int64_t Compress(char* src_buf, uint64_t src_len, char* dst_buf);
        void Decompress(char* src_buf, int64_t src_len, char* dst_buf, uint64_t dst_len);

        void resolve_functions();
        Zlib(std::string d_f)
        {
            if (d_f != "")
                dll_path = d_f;
            load_dll(dll_path);
        }


};

#endif