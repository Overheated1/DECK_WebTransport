# image types, codec applied and frames container

## Two kinds of distribution
(a) standard series of monoframe files (part 10) instances
(b) specialized "enhanced" series of multiframes file (part 10)
(c) specialized Legacy Converted Enhanced CT (upgrade of a into b)

When possible, the PCS shall transform (a) into (b), identified by SOP Class (c), with three major benefits:
- lower number of files
- less duplication of metadata
- continuous pixel space in storage
This applies to:
1.2.840.10008.5.1.4.1.1.2   [CT](https://dicom.nema.org/medical/dicom/current/output/html/part03.html#sect_A.3) -> 1.2.840.10008.5.1.4.1.1.2.2 [Legacy Converted Enhanced CT](https://dicom.nema.org/medical/dicom/current/output/html/part03.html#sect_A.70)
1.2.840.10008.5.1.4.1.1.4   [MR](https://dicom.nema.org/medical/dicom/current/output/html/part03.html#sect_A.4) -> 1.2.840.10008.5.1.4.1.1.4.4 [Legacy Converted Enhanced MR](https://dicom.nema.org/medical/dicom/current/output/html/part03.html#sect_A.71)
1.2.840.10008.5.1.4.1.1.128 [PT](https://dicom.nema.org/medical/dicom/current/output/html/part03.html#sect_A.56) -> 1.2.840.10008.5.1.4.1.1.128.1 [Legacy Converted Enhanced PET Image IOD](https://dicom.nema.org/medical/dicom/current/output/html/part03.html#sect_A.72)

In other DICOM classes, no formal conversion to multiframe was designed. Multiple instances are necessary, each one containing an image numbered cardinally starting at InstanceNumber 1.

## Three basic types
- series made of one **slide** frame only (direct radiography CR, DR, traditional 4 images MG)
- series made of **stream** frames (MG tomosyntesis, US, XA sequence)
- series made of **slices** frames (CT, MR, PT, XA 3D)

### slide
progressive resolution codec jxl.
When multiple instances to be compared in half screen exist within an unique series, new series are created each of which contain only one image.
This is important to manage correctly hanging protocols.
The jxl is kept within the series folder.

### stream mf.mp4
av1 inside an .mp4 container. Matroska .mtk may be better but is not as universal as mp4.
All the frames of the multiframe file, or all the images in separated instances are joined into an .mp4 container 
found within the series.


### slice mf.mxf
codec htj2k inside mxf container.
All the frames of the multiframe file, or all the images in separated instances are joined into an .mxf container
found within the series.


#### htj2k

https://ds.jpeg.org/whitepapers/jpeg-htj2k-whitepaper.pdf
- HTJ2K brings  more than 30x increase in throughput to JPEG 2000.
- It fully supports resolution scalability, efficient spatial random access,
multi-spectral and hyper-spectral content, and high throughput non-iterative precise rate control. 
- the accompanying JPH file format updates JPEG 2000 formats with modern colour space support,
  supporting HDR content with almost unlimited precision, while also allowing for the representation of raw colour
  sensor data with custom colour filter array patterns
- the HTJ2K standard is intended to be royalty free
- The HTJ2K standard defines a wrapping file format known as JPH, which extends the JP2 file format with support
  for modern parameterizable colour spaces, images without any colour space, and custom colour mappings
  suitable for raw sensor compression.

#### MXF
- SMPTE has added support for HTJ2K in MXF files in a recent revision of its SMPTE ST 422 standard (https://pub.smpte.org/doc/st422/20191213-pub/). 
  - This mapping shall use the MXF generic container in either the frame-based wrapping or clip-based wrapping mode defined in SMPTE ST 379-1 and SMPTE ST 379-2.
  - https://masv.io/blog/what-is-an-mxf-file
  - An open source implementation is available (https://github.com/sandflow/jid)

grouping
````
kdu_v_compress -i ~/Downloads/tiff-files/mer_shrt_23976_vdm_sdr_rec709_g24_3840x2160_20170913_12bit_DCDM.00090000.tif+100 -o - Corder="CPRL" Clevels="{6}" \
Cprecincts="{256,256},{256,256},{256,256},{256,256},{256,256},{256,256},{128,128}" ORGgen_tlm="{3}" ORGtparts=C Cblk="{32,32}" Creversible=yes Cmodes=HT -in_prec 12M \
  | jid-writer --format MJC --out ~/Downloads/part15-r.mxf
````
separating
````
mkdir ~/Downloads/j2c-out
jid-reader --in ~/Downloads/part15-r.mxf --format J2C --out ~/Downloads/j2c-out
````

- OpenJPH (https://github.com/aous72/OpenJPH) is an open source implementation of the HTJ2K standard. It includes an encoder and decoder
  implemented in C++ and a lightweight JavaScript decoder for web applications