#ifndef TABLES_H
#define TABLES_H
#define OBJECT_T int32_t // Resolve from Export Table
#define NAME_T int32_t // Resolve from Name Table

#include <iostream>
#include <fstream>

class NameTableEntry{
    public:
        uint32_t name_length;
        char* name;
        uint32_t id;

        void read(std::ifstream& fin)
        {
            char read_arr[4];
            fin.read(read_arr, 4);
            memcpy(&name_length, read_arr, 4);
            name = new char[name_length];
            fin.read(name, name_length);
        }

        friend std::ostream& operator<<(std::ostream& cout, NameTableEntry obj)
        {
            cout<<obj.id<<": ";
            for (uint32_t i = 0; i < obj.name_length; i++)
            {
                cout<<obj.name[i];
            }
            return cout;
        }
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
        } __attribute__((packed)) info;
        uint32_t id;

        void read(std::ifstream& fin)
        {
            char* read_array = new char [sizeof(info)];
            fin.read(read_array, sizeof(info));
            memcpy(&info, read_array, sizeof(info));
        }

        friend std::ostream& operator<<(std::ostream& cout, ExportTableEntry obj)
        {
            cout<<"Entry #"<<obj.id<<":"<<std::endl;
            cout<<"\tClass: "<<obj.info.object_class<<" ()"<<std::endl; //Populate with Class Name Later
            cout<<"\tSuper Class: "<<obj.info.object_super_class<<" ()"<<std::endl;
            cout<<"\tOuter Class: "<<obj.info.object_outer_class<<" ()"<<std::endl;
            cout<<"\tName: "<<obj.info.object_name<<" ()"<<std::endl;
            cout<<"\tobject_name_suffix: "<<obj.info.object_name_suffix<<" ()"<<std::endl;
            cout<<"\tobjects_flags: "<<obj.info.objects_flags<<" ()"<<std::endl;
            cout<<"\tobject_main_package: "<<obj.info.object_main_package<<" ()"<<std::endl;
            cout<<"\tunk1: "<<obj.info.unk1<<" ()"<<std::endl;
            cout<<"\tobject_size: "<<obj.info.object_size<<" ()"<<std::endl;
            cout<<"\tobject_offset: "<<obj.info.object_offset<<" ()"<<std::endl;
            cout<<"\tunk2: "<<obj.info.unk2<<" ()"<<std::endl;
            cout<<"\tunk3: "<<obj.info.unk3<<" ()";

            return cout;
        }
            
};

class ImportTableEntry{
    public:
        struct{
            OBJECT_T object_outer_package;
            NAME_T object_class_package;
            NAME_T class_name; // This could be Object_Name, I can't be sure, but it doesn't matter as I'm not gonna modify this, nor am I gonna extract anything.
            NAME_T class_name_suffix;
            NAME_T object_name;
        } __attribute__((packed)) info;
        uint32_t id;
    
        void read(std::ifstream& fin)
        {
            char* read_array = new char [sizeof(info)];
            fin.read(read_array, sizeof(info));
            memcpy(&info, read_array, sizeof(info));
        }

        friend std::ostream& operator<<(std::ostream& cout, ImportTableEntry obj)
        {
            cout<<"Entry #"<<obj.id<<": "<<std::endl;
            cout<<"\tOuter Package: "<<obj.info.object_class_package<<std::endl;
            cout<<"\tClass Name: "<<obj.info.class_name<<std::endl;
            cout<<"\tClass Name Suffix: "<<obj.info.class_name_suffix<<std::endl;
            cout<<"\tObject Name: "<<obj.info.object_name;

            return cout;
        }
};

#endif