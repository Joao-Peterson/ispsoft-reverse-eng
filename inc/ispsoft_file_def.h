#ifndef _ISPSOFT_FILE_DEF_
#define _ISPSOFT_FILE_DEF_

#include <stdint.h>

/* --------------------------------------- Enum's --------------------------------------- */

typedef enum{
    // VFD
    VFD_E_TYPE = 0x1E,
    VFD_MS300 = 0x23,
    VFD_MH300 = 0x22,
    VFD_C200 = 0x20,
    VFD_C2000_C2000Plus_CH2000_CT2000 = 0x1F,
    VFD_CP2000 = 0x21,
    // DVP
    ES_EC = 0x04,
    EX = 0x03,
    SS = 0x06,
    SA = 0x0C,
    SX = 0x0E,
    SC = 0x16,
    EH = 0x14,
    EH2 = 0x15,
    SV = 0x17,
    EH2_L = 0x18,
    ES2 = 0x08,
    ES2_E = 0x1D,
    EX2 = 0x09,
    SX2 = 0x10,
    SA2 = 0x11,
    MC =  0x12,
    SS2 = 0x0F,
    EH3 = 0x19,
    SV2 = 0x1A,
    EH3_L = 0x1B,
    SE = 0x13,
    EC3_8K = 0x18,
    ES3 = 0x0ED8,
    _15MC = 0x0E74,
    _50MC = 0x0E75,
    _15MC_06 = 0x0E76,
    _50MC_06 = 0x0E77,
    // TP
    TP04P = 0x67,
    TP70P_IO = 0x68,
    TP70P_RM = 0x69,
    // AS
    AS332T = 0x0BB8,
    AS332P = 0x0BB9,
    AS324MT = 0x0BBA,
    AS320T = 0x0BBB,
    AS320P = 0x0BBC,
    AS300N = 0x0BBD,
    AS228T = 0x0BC2,
    AS228P = 0x0BC3,
    AS228R = 0x0BC4,
    AS218TX = 0x0BC5,
    AS218PX = 0x0BC6,
    AS218RX = 0x0BC7
}ISP_PLC_TYPE;

/* --------------------------------------- Structs -------------------------------------- */

#pragma pack(push,1)

typedef struct{
    uint8_t     pad1[12]; // padding...
    uint16_t    version_ispsoft;
    uint8_t     pad2[2];
    uint32_t    plc_type;
    uint8_t     pad3[68];
    uint32_t    footer_offset;
    uint8_t     footer_size;
    uint8_t     pad4;
    uint8_t     file_identification; // always 0 (critical)
    uint8_t     check_value3; // always 1 (non critical)
    uint8_t     pad5[38];
    uint16_t    check_value2; // always 0 (critical)
    uint8_t     pad6[12];
    uint32_t    program_data_size;
    uint32_t    program_data_offset;
    uint32_t    compiled_data_size;
    uint32_t    check_value5; // always 0 (non critical)
}ISP_HEADER_t;

typedef struct{
    uint8_t     data[40]; // arbitrary data as it seens
}ISP_FOOTER_t;

#pragma(pop)

/* --------------------------------------- Globals -------------------------------------- */

uint8_t raw_footer[] = {0x00,0x02,0xFF,0x02,0x00,0x08,0xFF,0x0F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x70,0x00,0x7F,0x00,0xE0,0x00,0xFE,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x7F,0x00,0x98,0x01,0x57,0x02,0xD0,0x07,0x4F,0x0F};

ISP_FOOTER_t *footer_default = (ISP_FOOTER_t*)raw_footer;

#endif