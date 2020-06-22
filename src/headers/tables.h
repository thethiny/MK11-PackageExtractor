#ifndef TABLES_H
#define TABLES_H
#define OBJECT_T int32_t // Resolve from Export Table
#define NAME_T int32_t // Resolve from Name Table

#include <iostream>
#include <fstream>
#include <sstream>

class TableEntry{
    public:
        virtual void read(std::ifstream&) = 0;
        virtual std::string get_fullname() {return "None";}
        virtual std::string get_fullpath() {return "/";}
        virtual TableEntry* get_outer() {return NULL;}
};

class NameTableEntry: public TableEntry{
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
            cout<<obj.id<<": "<<obj.name;
            return cout;
        }
};

class NameTable{
    public:
        uint32_t entries_count;
        uint64_t location;
        NameTableEntry* entries;

        void read(std::ifstream& fin)
        {
            entries = new NameTableEntry [entries_count];
            fin.seekg(location);

            for (uint32_t i = 0; i < entries_count; i++)
            {
                entries[i].read(fin);
                entries[i].id = i;
            }
        }
        
        friend std::ostream& operator<<(std::ostream& cout, NameTable obj)
        {
            for (uint32_t i = 0 ; i < obj.entries_count; i++)
            {
                cout<<obj.entries[i]<<std::endl;
            }
            return cout;
        }
        
};

class ExportTableEntry: public TableEntry{
    public:
        struct{
            OBJECT_T object_class;
            OBJECT_T object_outer_class; // Package/Object this Object resides in
            NAME_T object_name; // Name of export
            uint32_t object_name_suffix; // When duplicate names exist, this suffix-1 is the name of the item. Eg: suffix 1 with name Hi -> Hi_0
            OBJECT_T object_super_class;
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

        struct
        {
            std::string object_fullname;
            TableEntry* object_class_ptr;
            TableEntry* object_outer_ptr;
            TableEntry* object_super_ptr;
            std::string object_mainpackage;
            std::string object_fullpath;
        } objects;
        

        void read(std::ifstream& fin)
        {
            char* read_array = new char [sizeof(info)];
            fin.read(read_array, sizeof(info));
            memcpy(&info, read_array, sizeof(info));
        }

        std::string get_fullname()
        {
            return objects.object_fullname;
        }

        std::string get_fullpath()
        {
            return objects.object_fullpath;
        }

        TableEntry* get_outer()
        {
            if (objects.object_outer_ptr)
                return objects.object_outer_ptr;
            return NULL;
        }

        friend std::ostream& operator<<(std::ostream& cout, ExportTableEntry obj)
        {
            cout<<"Entry #"<<obj.id<<":"<<std::endl;

            cout<<"\tClass: "<<obj.info.object_class;
            if (obj.objects.object_class_ptr)
                cout<<" ("<<obj.objects.object_class_ptr->get_fullname()<<")";
            cout<<std::endl; //Populate with Class Name Later

            cout<<"\tOuter Class: "<<obj.info.object_outer_class;
            if (obj.objects.object_outer_ptr)
                cout<<" ("<<obj.objects.object_outer_ptr->get_fullname()<<")";
            cout<<std::endl;

            cout<<"\tName: "<<obj.info.object_name<<" ("<<obj.objects.object_fullname<<")"<<std::endl;
            cout<<"\tName Suffix: "<<obj.info.object_name_suffix<<std::endl;

            cout<<"\tSuper Class: "<<obj.info.object_super_class;
            if (obj.objects.object_super_ptr)
                cout<<" ("<<obj.objects.object_super_ptr->get_fullname()<<")";
            cout<<std::endl;

            cout<<"\tObject Flags: "<<obj.info.objects_flags<<" ()"<<std::endl;

            cout<<"\tHeader Package Name: "<<obj.info.object_main_package<<" ("<<obj.objects.object_mainpackage<<")"<<std::endl;
            cout<<"\tunk1 (prob suffix): "<<obj.info.unk1<<std::endl;
            cout<<"\tSize: "<<obj.info.object_size<<std::endl;
            cout<<"\tOffset: "<<obj.info.object_offset<<std::endl;
            cout<<"\tunk2: "<<obj.info.unk2<<" ()"<<std::endl;
            cout<<"\tunk3: "<<obj.info.unk3<<" ()"<<std::endl;
            cout<<"\tFull Path: "<<obj.objects.object_fullpath;

            return cout;
        }
            
};

class ExportTable{
    public:
        uint32_t entries_count;
        uint64_t location;
        ExportTableEntry* entries;

        void read(std::ifstream& fin)
        {
            entries = new ExportTableEntry [entries_count];
            fin.seekg(location);

            for (uint32_t i = 0; i < entries_count; i++)
            {
                entries[i].read(fin);
                entries[i].id = i;
            }
        }

        friend std::ostream& operator<<(std::ostream& cout, ExportTable obj)
        {
            for (uint32_t i = 0 ; i < obj.entries_count; i++)
            {
                cout<<obj.entries[i]<<std::endl;
            }
            return cout;
        }
        
};

class ImportTableEntry: public TableEntry{
    public:
        struct{
            OBJECT_T object_outer_package; // I'm sure of this one
            NAME_T object_class_name; // I'm not sure about this one, but this is what Export Table needs
            NAME_T object_class_name_suffix; // I'm sure this is suffix
            NAME_T object_class_package; // Sure about this one
            NAME_T object_name; // I'm not sure about this one
        } __attribute__((packed)) info;
        uint32_t id;

        struct {
            TableEntry* object_outer_ptr;
            std::string object_class_full_name;
            std::string object_class_package_name;
            std::string object_name;
            std::string object_fullpath;
        } objects;
    
        void read(std::ifstream& fin)
        {
            char* read_array = new char [sizeof(info)];
            fin.read(read_array, sizeof(info));
            memcpy(&info, read_array, sizeof(info));
        }

        std::string get_fullname()
        {
            return objects.object_class_full_name;
        }

        std::string get_fullpath()
        {
            return objects.object_fullpath;
        }

        TableEntry* get_outer()
        {
            if (objects.object_outer_ptr)
                return objects.object_outer_ptr;
            return NULL;
        }

        friend std::ostream& operator<<(std::ostream& cout, ImportTableEntry obj)
        {
            cout<<"Entry #"<<obj.id<<": "<<std::endl;
            cout<<"\tOuter Package: "<<obj.info.object_outer_package;
            if (obj.objects.object_outer_ptr)
                cout<<" ("<<obj.objects.object_outer_ptr->get_fullname()<<")";
            cout<<std::endl;
            cout<<"\tClass Name: "<<obj.info.object_class_name<<" ("<<obj.get_fullname()<<")"<<std::endl;
            cout<<"\tClass Name Suffix: "<<obj.info.object_class_name_suffix<<std::endl;
            cout<<"\tClass Package: "<<obj.info.object_class_package<<" ("<<obj.objects.object_class_package_name<<")"<<std::endl;
            cout<<"\tObject Name: "<<obj.info.object_name<<" ("<<obj.objects.object_name<<")"<<std::endl;
            cout<<"\tFull Path: "<<obj.objects.object_fullpath;

            return cout;
        }
};

class ImportTable{
    public:
        uint32_t entries_count;
        uint64_t location;
        ImportTableEntry* entries;

        void read(std::ifstream& fin)
        {
            entries = new ImportTableEntry [entries_count];
            fin.seekg(location);
            
            for (uint32_t i = 0; i < entries_count; i++)
            {
                entries[i].read(fin);
                entries[i].id = i;
            }
        }

        friend std::ostream& operator<<(std::ostream& cout, ImportTable obj)
        {
            for (uint32_t i = 0 ; i < obj.entries_count; i++)
            {
                cout<<obj.entries[i]<<std::endl;
            }
            return cout;
        }
        
};

#endif