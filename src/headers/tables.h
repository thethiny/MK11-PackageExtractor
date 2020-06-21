#ifndef TABLES_H
#define TABLES_H
#define OBJECT_T int32_t // Resolve from Export Table
#define NAME_T int32_t // Resolve from Name Table

#include <iostream>

class NameTableEntry{
    public:
        uint32_t name_length;
        char* name;
};

class ExportTableEntry{
    public:
        struct{
            OBJECT_T object_class;
            OBJECT_T object_super_class;
            OBJECT_T object_outer_class; // Package/Object this Object resides in
            NAME_T object_name; // Name of export
            uint32_t object_name_suffix; // When duplicate names exist, this suffix-1 is the name of the item. Eg: suffix 1 with name Hi -> Hi_0
            uint64_t objects_flags; // Object flags
            char object_guid[0x10]; // Object GUID
            uint32_t object_main_package; // Main Package Name (From XXX file)
            uint32_t unk1;
            uint32_t object_size; // Size of Data
            uint64_t object_offset; // Location of Data in Decompressed File
            uint64_t unk2;
            uint32_t unk3;
        } info;
            
};

class ImportTableEntry{
    public:
        struct{
            OBJECT_T object_outer_package;
            NAME_T object_class_package;
            NAME_T class_name; // This could be Object_Name, I can't be sure, but it doesn't matter as I'm not gonna modify this, nor am I gonna extract anything.
            NAME_T class_name_suffix;
            NAME_T object_name;

        } info;
    
};

#endif