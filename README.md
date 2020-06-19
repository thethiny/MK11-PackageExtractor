# MK11-PackageExtractor
This is an application meant to extract the MK11 Package (.XXX &amp; .PSF) files.

# Info on XXX files:

The .xxx package is a modified UE4 package that looks as follows:

## Header

| Name                                      | Size |    Value    |
|-------------------------------------------|:-----:|:-----------:|
| Magic                                     |   4   | C1 83 2A 9E |
| File Version                              |   2   |    01 03    |
| Licensee Version                          |   2   |    9D 00    |
| Decompressed Data Start Location          |   4   |             |
| Shader Version                            |   4   | 2E 0A 00 00 |
| Engine Version                            |   4   | E7 01 00 00 |
| Midway Team's Four CC (Magic)             |   4   |    "MK11"   |
| Midway Team's Engine Version              |   4   | 50 00 00 00 |
| Cook Version                              |   4   | 4B 04 00 00 |
| Main Package                              |   4   |    "MAIN"   |
| Package Flags                             |   4   |             |
| Name Table Entries                        |   4   |             |
| Name Table Offset in Decompressed Data    |   8   |             |
| Export Table Entries                      |   4   |             |
| Export Table Offset in Decompressed Data  |   8   |             |
| Import Table Entries                      |   4   |             |
| Import Table Offset in Decompressed Data  |   8   |             |
| BulkData Offset in Decomrpessed Data      |   8   |             |
| MD5                                       |   16  |             |
| Compression Flag                          |   4   |             |
| Number of Packages                        |   4   |             |

## Packages

For each Package in Package Count

| Name                                   |         Size        |  Value |
|----------------------------------------|:-------------------:|:------:|
| Package Name Length                    |          4          |        |
| Package Name                           | Package Name Length | String |
| Offset in Decompressed Data            |          8          |        |
| Package Decompressed Buffer Total Size |          8          |        |
| Package Location                       |          8          |        |
| Package Size                           |          8          |        |
| SubPackages Count                      |          4          |        |

## Sub Packages

For each Sub Package in Sub Packages Count

| Name                         | Size | Value |
|------------------------------|:----:|:-----:|
| Offset in Decompressed Data  |   8  |       |
| Decompressed SubPackage Size |   8  |       |
| SubPackage Start Location    |   8  |       |
| SubPackage Size              |   8  |       |

## After all Packages are done

| Name                         | Size | Value |
|------------------------------|:----:|:-----:|
| Additional Packages Count    |   4  |       |

Repeat Package for each additional Package

## After all additional Packages

| Name                    |                  Size                 |  Value      |
|-------------------------|:-------------------------------------:|:-----------:|
| Padding                 |                  0x18                 |     00      |
| File Name Length        |                   4                   |             |
| File Name               |            File Name Length           |   String    |
| PSF Table Count         |                   4                   |             |
| PSF Table               |                                       | Extra Table |
| Bulk Data Table Count   |                   4                   |             |
| Bulk Data Table         |                                       | Extra Table |

PSF Table and Bulk Table are both of the type Extra Table

## Extra Table(s)

| Name                        |     Size    |  Value |
|-----------------------------|:-----------:|:------:|
| UNK1                        |      4      |        |
| UNK2                        |      4      |        |
| Name Length                 |      4      |        |
| Name                        | Name Length | String |
| Entries Count               |      4      |        |

### Extra Table Entry
| Name                        |     Size    |  Value |
|-----------------------------|:-----------:|:------:|
| Decompressed Size           |      8      |        |
| Compressed Size             |      8      |        |
| Offset in Decompressed file |      8      |        |
| Offset in Compressed File   |      8      |        |

Repeat for `Entries Count`

| Name                        |     Size    |  Value |
|-----------------------------|:-----------:|:------:|
| Compression Flag            |      4      |        |

If Compression Flag is 0, Compressed Size/Offset is -1.
If Compression Flag is not 0 but the data is in a separate file (example, PSF file), then Decompressed/Compressed Offset are the same.
The point of Decompressed/Compressed offsets are the location within the upk file, since the PSF file doesn't have to be converted to UPK, then the UPK offset is similar to the PSF offset.



## Segments
| Name                              | Size | Value       |
|-----------------------------------|:----:|:-----------:|
| Magic                             | 4    | C1 83 2A 9E |
| Unknown                           | 4    |             |
| Maximum Segment Decompressed Size | 8    |             |
| Total Segment Compressed Size     | 8    |             |
| Total Segment Decompressed Size   | 8    |             |

## Compressed Segments

While Total Segment Size (compressed/decompressed) not fulfilled:

| Name                      | Size | Value       |
|---------------------------|:----:|:-----------:|
| Segment Compressed Size   | 8    |             |
| Segment Decompressed Size | 8    |             |

## Data

| Name   | Size                    | Value  |
|--------|:-----------------------:|:------:|
| Data   | Segment Compressed Size |        |

Repeat for number of Compressed Segments

After all segments done, repeat from [Segments](#segments) until file is done.



# Todo

- XXX File Header
  - [x] Reading
  - [x] Understanding
- XXX File Packages
  - [x] Reading
  - [x] Understanding
- XXX File SubPackages
  - [x] Reading
  - [x] Understanding
- XXX File Oodle Segments
  - [x] Reading
  - [x] Understanding
  - [x] Extracting
- PSF Files
  - [x] Reading
  - [x] Understanding
  - [x] Extracting
- XXX File Footer
  - [x] Reading
  - [x] Understanding
- XXX File Footer Extra Info
  - [X] Reading PSF Table
  - [X] Understanding PSF Table
  - [X] Reading Bulk Table
  - [X] Understanding Bulk Table
  - [ ] Understanding the reason behind the 0x18 padding
  - [ ] Understanding the reason behind the 0x08 Padding before file internal name
- HeaderData Package of XXX File
  - [x] Understanding
  - [ ] Mapping Name/Import/Export Tables
- Extra
  - [x] Add flag to skip PSF
  - [x] Allow file input via cli
  - [x] Change file input to load by location not by folder "Files"
  - [ ] Generate info to enable repacking of files
  - [ ] Extracted files should have proper names
  - [x] Oodle Decompression
  - [x] Oodle Compression
  - [x] Zlib Decompression
  - [ ] Zlib Compression
  - [x] Convert XXX into UPK


# How to build

You need GCC Compiler because I'm using [`__attribute__((packed))`](http://www.keil.com/support/man/docs/armclang_ref/armclang_ref_chr1359124982232.htm#:~:text=__attribute__((packed))%20variable%20attribute,-Home%20%C2%BB%20armclang%20Reference&text=You%20can%20specify%20the%20packed,has%20the%20smallest%20possible%20alignment.).

You also need the x64 version since Decompression relies on the x64 space.

I'm importing `windows.h` therefore it'll fail in Linux. Swap for Mac/Linux equivalent on your machine.

For Visual Code, download the C++ Intellisense extension and add a build task to your config to automatically include all the includes and all the local files to match them together. Also add src/headers to your include dir as well as the standard libraries to your `.vscode/c_cpp_properties.json` if you want to edit in Visual Code for linking.

_See reference [`.vscode/tasks.json`](/.vscode/tasks.json) for example._
