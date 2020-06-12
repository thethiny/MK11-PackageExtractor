#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <iostream>
#include <windows.h>

class Compression{
    public:
        HINSTANCE dll_object;

        virtual int64_t Compress(char* src_buf, uint64_t src_len, char* dst_buf) = 0;
        virtual void Decompress(char* src_buf, int64_t src_len, char* dst_buf, uint64_t dst_len) = 0;
        void load_dll(std::string dll_path);
        virtual void resolve_functions() = 0;

};

#endif