#ifndef NES_MEMORY_H
#define NES_MEMORY_H

#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdint>

#include "CPU.h"

# define MAX_MEM 0xFFFF

class Memory {
public:
    Memory();

    void loadBinary(const char* filename);
    uint16_t calc_addr(addr_mode mode, cpu_state state);
    uint8_t read(uint16_t addr);
    uint16_t read16(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
    void dump();

    void reset();

private:
    uint8_t mem[MAX_MEM]{}; // 16K

};

#endif //NES_MEMORY_H
