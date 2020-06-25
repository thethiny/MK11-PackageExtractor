#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <direct.h>
#include <windows.h>

using namespace std;

int main()
{
    string path = "output\\GEARASSETS_KIT_ScriptAssets\\PSFData\\1_Texture";
    uint64_t i = 0;
    stringstream file;
    file<<path<<"\\"<<i<<".decompressed";
    ifstream fin(file.str().c_str(), ios::binary);
    fin>>noskipws;
    while (fin.good())
    {
        fin.seekg(0, ios::end);
        uint64_t file_size = fin.tellg();
        uint32_t width = uint32_t(sqrt((float)file_size));
        stringstream file_out;
        file_out<<path<<"\\"<<i<<".DDS";
        ofstream fout(file_out.str().c_str(), ios::binary);
        fout<<"DDS ";

        uint32_t bytes_4 = 0x7C;
        fout.write((char*)&bytes_4, 4);

        bytes_4 = 0x1007;
        fout.write((char*)&bytes_4, 4);

        fout.write((char*)&width, 4);
        fout.write((char*)&width, 4);
        fout.write((char*)&file_size, 4);

        bytes_4 = 0;
        fout.write((char*)&bytes_4, 4);

        bytes_4 = 1;
        fout.write((char*)&bytes_4, 4);
        for (int i = 0; i < 11; i++)
        {
            bytes_4 = 0;
            fout.write((char*)&bytes_4, 4);
        }
        bytes_4 = 0x20;
        fout.write((char*)&bytes_4, 4);

        bytes_4 = 0x4;
        fout.write((char*)&bytes_4, 4);

        fout<<"DX10";

        for (int i = 0; i < 10; i++)
        {
            bytes_4 = 0;
            fout.write((char*)&bytes_4, 4);
        }

        bytes_4 = 0x62;
        fout.write((char*)&bytes_4, 4);

        bytes_4 = 0x3;
        fout.write((char*)&bytes_4, 4);

        bytes_4 = 0;
        fout.write((char*)&bytes_4, 4);

        bytes_4 = 1;
        fout.write((char*)&bytes_4, 4);

        bytes_4 = 0;
        fout.write((char*)&bytes_4, 4);

        fin.seekg(0, ios::beg);
        
        char* data = new char [file_size];
        fin.read(data, file_size);

        fout.write(data, file_size);

        fin.close();
        fout.close();

        file.str(string());
        file.clear();
        i++;
        file<<path<<"\\"<<i<<".decompressed";
        cout<<"FILE: "<<file.str()<<endl;;
        fin.open(file.str().c_str(), ios::binary);
        fin>>noskipws;
    }
}