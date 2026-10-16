// project: dicm2deck
// targets: dicm2cda,dicmstructdump,...
// file: uapi.h
// created by jacquesfauquex on 2024-04-04.

#ifndef uapi_h
#define uapi_h

#include "datatypes.h"


#pragma mark - propietary vr

enum kvVRcategory{
    //vl enum
    kvFD,// 0 FD floating point double
    kvFL,// 1 FL floating point single
    kvSL,// 2 SL signed long
    kvSS,// 3 SS signed short
    kvUL,// 4 UL unsigned long
    kvUS,// 5 US unsigned short
    kvAT,// 6 AT attribute tag, 2 u16 hexa
    kvUI,// 7 UI unique ID possibly padded with 0x00
    kvUe,// 8 Ue unique ID possibly padded with 0x00 for tag 0020000D study UID
    kvUs,// 9 Us unique ID possibly padded with 0x00 for tag 0020000E series UID
    kvUi,//10 Ui unique ID possibly padded with 0x00 for tag 00080018 Instance UID
    kvUc,//11 Ui unique ID possibly padded with 0x00 for tag 00080016 Class UID
    kvUp,//12 Up unique ID possibly padded with 0x00 for tag 00080019 Pyramid UID
    kvTP,//13 AS DT TM DA text short ascii pair length
    kvTA,//14 AE DS IS 13 text short ascii
    kvCS,//15 CS Code String
    kvCs,//16 Cs Code String 00080005 read in main to enable charset control
    kvTS,//17 LO LT SH ST text short charset
    kvPN,//18 PN person name has a special treatment in json and xml
    kv01,//19 other OB OD OF OL OV OW SV UV binary except pixel
    kvTL,//20 UC UT text long charset
    kvTU,//21 UR text long url-encoded
    kvSA,//22 SQ head with length 0xffffffff terminated by end tag
    kvSa,//23 SQ head with specified length
    kvIA,//24 item head with length 0xffffffff terminated by end tag
    kvIa,//25 item head with specified length
    kvIZ,//26 item tail tag of item of undefined size
    kvIz,//27 item tail added
    kvSZ,//28 SQ tail tag of SQ of undefined size
    kvSz,//29 SQ tail added
    kvUN,//30 UN
    kPDF,//31
    kCDA,//32
    kSTL,//33
    kOBJ,//34
    kMTL,//35
    kPIX//36 0x7FE00010 OB / OW
    };


void uinput(int argc, char *argv[]);//opening of DICM
void utrail(int argc, char *argv[]);//after dataset read

void key(struct Ercle* attr);//returns little endian of the key or 0 if error
void val(enum kvVRcategory vrcat,struct Ercle* attr);

#endif /* uapi_h */
