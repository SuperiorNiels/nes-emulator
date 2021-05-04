#ifndef NES_RAMBUS
#define NES_RAMBUS

#include "Bus.h"
#include "Memory.h"

class RAM_Bus : public Bus {
public:
    RAM_Bus() { RAM = new Memory(0xFFFF); }

    void reset() override { RAM->reset(); };
    bool openROM(const char* filename) override { return RAM->openROM(filename); };
    uint8_t read(int64_t& cycles, uint16_t addr) override { return RAM->read(cycles, addr); };
    void write(int64_t& cycles, uint16_t addr, uint8_t data) override { RAM->write(cycles, addr, data); };
    
    ~RAM_Bus() override { delete RAM; };

    Memory* RAM = nullptr;
};

#endif