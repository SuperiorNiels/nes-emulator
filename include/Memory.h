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
    Memory() {};

    void reset();
    void loadBinary(const char* filename);
    uint16_t calc_addr(int64_t& cycles, addr_mode mode, cpu_state state);

    uint8_t read(int64_t& cycles, uint16_t addr);
    uint16_t read16(int64_t& cycles, uint16_t addr);
    void write(int64_t& cycles, uint16_t addr, uint8_t data);

    uint8_t* getMemoryStartPointer();
private:
    uint8_t mem[MAX_MEM]{}; // 16K

};

#endif //NES_MEMORY_H
