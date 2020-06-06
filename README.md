# MK11-PackageExtractor
This is an application meant to extract the MK11 Package (.XXX &amp; .PSF) files.

# Info on XXX files:

The .xxx package is a modified UE4 package that looks as follows:
## Header

4 Bytes Magic: C1 83 2A 9E

2 Bytes Version: 01 03 as of June 2020

2 Bytes Another Version: 9D 00 as of June 2020

4 Unk

4 Unk

4 Engine Version: E7 01 as of June 2020

4 Game Identifier: "MK11"

4? Another Version: 50 as of June 2020

4 UNK

4 "MAIN"

Uknown until 0x68

4 Packages Count

## Packages

For each Package in Package Count

4 Package Name Length

Package Name

8 Uknown

8 Total Decompressed Package Size

8 Package Start Location

8 Package Size

4 Sub Packages Count

## Sub Packages

For each Sub Package in Sub Packages Count

8 Unknown

8 Decompressed Sub Package size

8 Sub Package Start Location

8 Sub Package Size

then jump to the start location and you enter the Segments.

## Segments

[Add Segment info here]

then jump to compressed segment

## Compressed Segments

[Add Compressed Segment info here]


## After all Packages are done

4 Additional Packages Count

Repeat Package for each additional Package

## After all additional Packages

0x18 Padding (00)

4 File Name

File Name

Until Package 0 Start Offset contains some data that is yet to be understood. However 0x20 after the name seems to be a counter*2 of some sort.



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
  - [ ] Change file input to load by location not by folder "Files"