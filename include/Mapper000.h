#ifndef NES_MAPPER000
#define NES_MAPPER000

#include "Mapper.h"

class Mapper000 : public Mapper {
public:
    Mapper000() = delete;
    Mapper000(const uint8_t PRG_size, const uint8_t CHR_size) 
        : Mapper(PRG_size, CHR_size) {};

    virtual uint16_t getMappedPRG(uint16_t addr) override {
        return addr & (PRG_size > 1 ? 0x7FFF : 0x3FFF);
    };

    virtual uint16_t getMappedCHR(uint16_t addr) override {
        return addr;
    };
};

#endif