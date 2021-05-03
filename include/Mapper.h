#ifndef NES_MAPPER
#define NES_MAPPER

#include <stdint-gcc.h>

#include "Debug.h"

class Mapper {
public:
    Mapper() {};
    Mapper(const uint8_t PRG_size, const uint8_t CHR_size) 
        : PRG_size(PRG_size), CHR_size(CHR_size) {};

    virtual uint16_t getMappedPRG(uint16_t addr) = 0;
    virtual uint16_t getMappedCHR(uint16_t addr) = 0;

protected:
    uint8_t PRG_size = 1, CHR_size = 1;
};

#endif