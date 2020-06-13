#ifndef MK11HEADER_H
#define MK11HEADER_H

#include <iomanip>
#include "package.h"
#include "filehandle.h"
#include "utils.h"

class MK11File{
    private:
        struct VersionInfo{
            unsigned int magic = 0x9E2A83C1u;
            unsigned int engine_version = 0x01E7u;
            unsigned int version_3 = 0x50;
            unsigned short file_version = 0x0301u;
            unsigned short licensee_version = 0x9Du;
        } version_info;
        void validate_header();

    public:
        struct Info{
            uint32_t magic;
            uint16_t file_version;
            uint16_t licensee_version;
            uint32_t decompressed_start; // Location of Data in Total Decompressed Data
            uint32_t version_2; // Maybe
            uint32_t engine_version;
            char game_name[4];
            uint32_t version_3;
            uint32_t version_4;
            char main_package_name[4];
            uint32_t package_flags;
            uint32_t name_table_entries;
            uint64_t decompressed_header_location; // Location of Header in the Total Decompressed Data
            uint32_t export_table_entries;
            uint64_t decompressed_export_table_location;
            uint32_t import_table_entries;
            uint64_t decompressed_import_table_location;
            uint64_t decompressed_total_size; // Without files that have _BulkData suffix, need to figure out how to determine bulkdata, prob flag in expTbl.
            uint8_t file_GUID[0x10];
            uint32_t compression_flag; // Most Probably
            uint32_t number_of_packages;
        } __attribute__((packed)) info; 

        enum compression_flags
        {
            ZLIB  = 0x0001u,
            LZO   = 0x0002u,
            LZX   = 0x0004u,
            OODLE = 0x0100u
        };

        enum package_flags
        {
            PKG_None                          = 0x00000000,
            PKG_NewlyCreated                  = 0x00000001,
            PKG_ClientOptional                = 0x00000002,
            PKG_ServerSideOnly                = 0x00000004,
            PKG_CompiledIn                    = 0x00000010,
            PKG_ForDiffing                    = 0x00000020,
            PKG_EditorOnly                    = 0x00000040,
            PKG_Developer                     = 0x00000080,
            PKG_UncookedOnly                  = 0x00000100,
            PKG_ContainsMapData               = 0x00004000,
            PKG_Compiling                     = 0x00010000,
            PKG_ContainsMap                   = 0x00020000,
            PKG_RequiresLocalizationGather    = 0x00040000,
            PKG_PlayInEditor                  = 0x00100000,
            PKG_ContainsScript                = 0x00200000,
            PKG_DisallowExport                = 0x00400000,
            PKG_ReloadingForCooker            = 0x40000000,
            PKG_FilterEditorOnly              = 0x80000000,
        };

        enum object_flags
        {
            RF_NoFlags                         =0x00000000,
            RF_Public                          =0x00000001,
            RF_Standalone                      =0x00000002,
            RF_MarkAsNative                    =0x00000004,
            RF_Transactional                   =0x00000008,
            RF_ClassDefaultObject              =0x00000010,
            RF_ArchetypeObject                 =0x00000020,
            RF_Transient                       =0x00000040,
            RF_MarkAsRootSet                   =0x00000080,
            RF_TagGarbageTemp                  =0x00000100,
            RF_NeedInitialization              =0x00000200,
            RF_NeedLoad                        =0x00000400,
            RF_KeepForCooker                   =0x00000800,
            RF_NeedPostLoad                    =0x00001000,
            RF_NeedPostLoadSubobjects          =0x00002000,
            RF_NewerVersionExists              =0x00004000,
            RF_BeginDestroyed                  =0x00008000,
            RF_FinishDestroyed                 =0x00010000,
            RF_BeingRegenerated                =0x00020000,
            RF_DefaultSubObject                =0x00040000,
            RF_WasLoaded                       =0x00080000,
            RF_TextExportTransient             =0x00100000,
            RF_LoadCompleted                   =0x00200000,
            RF_InheritableComponentTemplate    =0x00400000,
            RF_DuplicateTransient              =0x00800000,
            RF_StrongRefOnFrame                =0x01000000,
            RF_NonPIEDuplicateTransient        =0x02000000,
            RF_Dynamic                         =0x04000000,
            RF_WillBeLoaded                    =0x08000000,
        };

        uint32_t number_of_extra_packages = 0;
        char* internal_file_name;
        bool has_psf;
        bool load_psf = true;

        Package* packages;
        Package* packages_extra;

        const uint64_t MAX_DEC_SIZE = 0x20000; // Maximum Size for a chunk of uncompressed data

        FileHandle* input_file_obj;

        void read(std::ifstream&);
        void read_footing(std::ifstream&);
        void read_extra_tables(std::ifstream&);
        void read_packages(std::ifstream&);
        void read_packages_extra(std::ifstream&);
        void read_compressed_segments(std::ifstream&);
        void read_compressed_segments_extra();
        void register_file(FileHandle& file) {input_file_obj = &file;}
        void set_psf_status(bool status) {has_psf = status && number_of_extra_packages && load_psf;}
        bool get_psf_status() {return has_psf;}
        void print();
        friend std::ostream &operator<<(std::ostream&, MK11File);

};

#endif