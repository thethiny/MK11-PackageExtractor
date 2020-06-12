#ifndef OODLE_H
#define OODLE_H
#include <iostream>
#include <windows.h>
#include "compression.h"

class Oodle: public Compression{
    public:
        std::string dll_path = "./oo2core_5_win64.dll";
        std::string compress_function_name = "OodleLZ_Compress";
        std::string decompress_function_name = "OodleLZ_Decompress";
        std::string default_compression_options_function_name = "OodleLZ_CompressOptions_GetDefault";

        typedef int WINAPI OodleLZ_CompressFunc(
            uint32_t codec, char *src_buf, uint64_t src_len, char *dst_buf, int64_t level,
            void *opts, int64_t offs);
        typedef int WINAPI OodleLZ_DecompressFunc(
            char *src_buf, int64_t src_len, char *dst_buf, uint64_t dst_size, int fuzz, int crc, int verbose,
            uint8_t *dst_base, size_t e, void *cb, void *cb_ctx, void *scratch, size_t scratch_size, int threadPhase
        );

        typedef void* WINAPI OodleLZ_CompressOptions_GetDefaultFunc(int64_t unused, int64_t level);

        OodleLZ_CompressFunc* oodl_compress;
        OodleLZ_DecompressFunc* oodl_decompress;
        OodleLZ_CompressOptions_GetDefaultFunc* oodle_get_default;

        void* default_compression_options;

        enum compression_types
        {
            mk11 = 7,
            kraken,
            mermaid,
            median,
            selkie,
            hydra,
            leviathan
        };

        static const uint32_t compression_type = compression_types::mk11;
        static const int64_t compression_level = 7;

        int64_t Compress(char* src_buf, uint64_t src_len, char* dst_buf);
        void Decompress(char* src_buf, int64_t src_len, char* dst_buf, uint64_t dst_len);
        
        void resolve_functions();
        Oodle(std::string d_f)
        {
            if (d_f != "")
                dll_path = d_f;
            load_dll(dll_path);
        }

};


#endif