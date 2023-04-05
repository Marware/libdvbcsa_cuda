
#include "dvbcsa_pv.h"

int csa_start()
{
    //Sample scrambled keys from a TS packet
    unsigned char data[16] = {0xD8,0xE5,0x55,0xCC,0xF0,0xF8,0x1C,0xFF,0xB6,0xE2,0xD9,0xA9,0x47,0x66,0x5F,0x19};

    //Sample key
    uint8_t cw[8] = {0x11, 0x11, 0x11, 0x33, 0xAA, 0xAA, 0xAA, 0xFE};

    csa_cu_decrypt(cw, data);

    return 0;
}
