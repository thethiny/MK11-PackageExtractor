# MK11-PackageExtractor
This is an application meant to extract the MK11 Package (.XXX &amp; .PSF) files.

# Info on XXX files:

The .xxx package is a modified UE4 package that looks as follows:

## Header

| Name               | Size |    Value    |
|--------------------|:----:|:-----------:|
| Magic              |   4  | C1 83 2A 9E |
| Engine Version 1/2 |   2  |    01 03    |
| Engine Version 2/2 |   2  |    9D 00    |
| Unknown            |   4  |             |
| Unknown            |   4  |             |
| Script Version     |   4  | E7 01 00 00 |
| Game Identifier    |   4  |    "MK11"   |
| ? Version          |   4  | 50 00 00 00 |
| Unknown            |   4  |             |
| Main Package       |   4  |    "MAIN"   |

Uknown until 0x68

4 Packages Count

## Packages

For each Package in Package Count

| Name                                   |         Size        |  Value |
|----------------------------------------|:-------------------:|:------:|
| Package Name Length                    |          4          |        |
| Package Name                           | Package Name Length | String |
| Unknown                                |          8          |        |
| Package Decompressed Buffer Total Size |          8          |        |
| Package Location                       |          8          |        |
| Package Size                           |          8          |        |
| SubPackages Count                      |          4          |        |

## Sub Packages

For each Sub Package in Sub Packages Count

| Name                         | Size | Value |
|------------------------------|:----:|:-----:|
| Unknown                      |   8  |       |
| Decompressed SubPackage Size |   8  |       |
| SubPackage Start Location    |   8  |       |
| SubPackage Size              |   8  |       |

## After all Packages are done

| Name                         | Size | Value |
|------------------------------|:----:|:-----:|
| PSF Packages Count           |   4  |       |

Repeat Package for each additional Package

## After all additional Packages

| Name                    |                  Size                 |  Value |
|-------------------------|:-------------------------------------:|:------:|
| Padding                 |                  0x18                 |   00   |
| File Name Length        |                   4                   |        |
| File Name               |            File Name Length           | String |
| Unknown PSF Table Count |                   4                   |        |
| PSF Table               | Unknown PSF Table Count * unk formula |        |
| Unknown Table Count     | 4                                     |        |
| Unknown Table           | Unknown Table Count * unk formula     |        |

Until Package 0 Start Offset contains some data that is yet to be understood. However 0x20 after the name seems to be a counter*2 of some sort.

Then jump to the start location and you enter the Segments.

## Segments
| Name                            | Size | Value       |
|---------------------------------|:----:|:-----------:|
| Magic                           | 4    | C1 83 2A 9E |
| Unknown                         | 4    |             |
| Unknown                         | 8    |             |
| Total Segment Compressed Size   | 8    |             |
| Total Segment Decompressed Size | 8    |             |

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

- [ ] XXX File Header
  - [x] Reading
  - [ ] Understanding
- [ ] XXX File Packages
  - [x] Reading
  - [x] Understanding
- [ ] XXX File SubPackages
  - [x] Reading
  - [x] Understanding
- [ ] XXX File Oodle Segments
  - [x] Reading
  - [x] Understanding
  - [ ] Extracting
- [ ] PSF Files
  - [x] Reading
  - [x] Understanding
  - [ ] Extracting
- [ ] XXX File Footer
  - [x] Reading
  - [x] Understanding
- [ ] XXX File Footer Extra Info
  - [ ] Reading PSF Table
  - [ ] Understanding PSF Table
  - [ ] Reading XXX Table
  - [ ] Understanding XXX Table
- [ ] HeaderData Package of XXX File
  - [ ] Understanding
  - [ ] Mapping Name/Import/Export Tables
- [ ] Extra
  - [x] Add flag to skip PSF
  - [x] Allow file input via cli
  - [x] Change file input to load by location not by folder "Files"