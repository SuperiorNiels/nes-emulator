#ifndef NES_NESMEMORY
#define NES_NESMEMORY


#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdint>

#include "Debug.h"
#include "Mapper.h"
#include "Mapper000.h"
#include "DirectMemory.h"

class NESMemory : public Memory {
public:
    NESMemory();

    void reset() override;
    uint8_t read(int64_t& cycles, uint16_t addr) override;
    void write(int64_t& cycles, uint16_t addr, uint8_t data) override;
    void loadROM(const char* filename) override;

    ~NESMemory() override;

public: // TODO make private and expose necessary data (public for debugging)
    uint8_t PRG_size = 0, CHR_size = 0;
    Mapper* mapper = nullptr;
    DirectMemory* PRG = nullptr;
    DirectMemory* CHR = nullptr;
    DirectMemory* PPU_registers = nullptr;
    DirectMemory* workRAM = nullptr;
};

#endif