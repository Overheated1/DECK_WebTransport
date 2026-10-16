# ESCIPFserialized

https://dicom.nema.org/dicom/2013/output/chtml/part03/chapter_A.html

Based on ESIPbasetags and KVserialized 
Serializes attributes into groups corresponding to a simplified model of information entities:
- exam (patient+study)
- series (+equipment)
- capsule (encapsulated instance content treated as series, that is, stored in the database when there is one)
- instance (everything else but frames)
- private

00080005 removed. Everything textual is always written using UTF-8 coding

## groups of attributes
ESCIPFserialized lists groups of KVattributes, which belong to one the predefined information entity.
The groups are prefixed by a head KV that we create. This head KV is not part of the original datataset.

DECK groups could be seen as a new and different implementation of the group length attributes in the original DICOM standard.
With respect to the latter, the differences are:
- attribute tag and vr (8 bytes) are replaced by a key length (1 byte) and a key (odd size less than 256)
- the key uses only url safe chars and slash and represent the route to a directory and a hash sum
- the group itself is essentially a macro of specifically related attributes, (instead of a gathering of tags with same first two bytes prefix).

````
• 1 byte: length of the key (KL)
• KL bytes: the key
• 4 bytes: length of the value (VL)
• VL bytes: the value, exactly as in the DICM
representation.
````
Formally, the head KV looks like any KV, with the exception the VL refers to the sum of all the KV attributes of the group. 
This length can be used as a pointer to the next one, which allows to skip parsing the current one. 

To be sure that the same information was already parsed and can be skipped, the key ends with a blake3 hash of the group. 

The encapsulated objects and frames are exceptions with no blake3. Being first class objects of other standards, 
the inner coherence is secured by the corresponding standard.
In these cases, the group does contain the encapsulated object (instead of a list of attributes). 
The object length is exact, with no padding null char in case the length is odd.

## group key components
| category             | key prefix               | key name           |  key suffix  | comment                       |
|----------------------|--------------------------|--------------------|--------------|-------------------------------|
| Exam+patient         | date/E/           (even) | --             (2) | .blake3 (65) |                               |
| Series               | date/E/S/          (odd) | ---            (3) | .blake3 (65) |                               |
| PDF                  | date/E/S/          (odd) | ps             (2) | .pdf    (4)  | 1.2.840.10008.5.1.4.1.1.104.1 |
| CDA                  | date/E/S/          (odd) | uc sc ds ot    (2) | .xml    (4)  | 1.2.840.10008.5.1.4.1.1.104.2 |
| STL                  | date/E/S/          (odd) | 3d             (2) | .stl    (4)  | 1.2.840.10008.5.1.4.1.1.104.3 |
| OBJ                  | date/E/S/          (odd) | tx             (2) | .obj    (4)  | 1.2.840.10008.5.1.4.1.1.104.4 |
| MTL                  | date/E/S/          (odd) | tx             (2) | .mtl    (4)  | 1.2.840.10008.5.1.4.1.1.104.5 |
| MXF                  | date/E/S/          (odd) | mf             (2) | .mxf    (4)  | codec jh inside mxf container |
| JXL                  | date/E/S/          (odd) | sf             (2) | .jxl    (4)  | codec jpeg xl monoframe       |
| MP4                  | date/E/S/          (odd) | mf             (2) | .mp4    (4)  | codec av1 inside mp4 container|
| Frames htj2k -> MXF  | date/E/S/H/       (even) | 00001..99999   (5) |              | frame file/image number       |
| Frames png -> AV1 mp4| date/E/S/P/       (even) | 00001..99999   (5) |              | frame file/image number       |
| Instance+sop+private | date/E/S/I/       (even) | priv ----      (4) | .blake3 (65) | private + 0002                |

- there shall exist only one of PDF, CDA, STL, OBJ, MTL, MXF, JXL or MP4 file within the series
- H and P receive individual frames in htj2k and png respectively from multiframes or multiple instances.
When the series is complete another process creates a mf.mxf (which gathers the htj2k) or a mf.mp4 (which gathers the png)
- date, E, S, I, codified uibb64 ("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz~") are url safe
of even length, lower than 45.
