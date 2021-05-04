#ifndef NES_NESMEMORY
#define NES_NESMEMORY


#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdint>

#include "Bus.h"
#include "Debug.h"
#include "Mapper.h"
#include "Mapper000.h"
#include "Memory.h"
#include "Cartridge.h"


class CPU_Bus : public Bus {
public:
    CPU_Bus() = delete;
    CPU_Bus(Cartridge* cartridge);

    void reset() override;
    bool openROM(const char* filename) override;
    uint8_t read(int64_t& cycles, uint16_t addr) override;
    void write(int64_t& cycles, uint16_t addr, uint8_t data) override;

    ~CPU_Bus() override;

public: // TODO make private and expose necessary data (public for debugging)
    Cartridge* cartridge = nullptr;
    Memory* cpuRAM = nullptr;
    Memory* PPU_registers = nullptr;
    Memory* workRAM = nullptr;
};

#endif