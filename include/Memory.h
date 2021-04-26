#ifndef NES_MEMORY_H
#define NES_MEMORY_H

#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdint>

#include "CPU.h"


class Memory {
public:
    Memory() {};
    explicit Memory(const uint32_t mem_size);

    void reset();
    void loadBinary(const char* filename);
    uint16_t calc_addr(int64_t& cycles, addr_mode mode, const cpu_state& state);

    uint8_t read(int64_t& cycles, uint16_t addr);
    uint16_t read16(int64_t& cycles, uint16_t addr);
    void write(int64_t& cycles, uint16_t addr, uint8_t data);

    uint8_t* getMemoryStartPointer();
    const uint32_t getMemorySize() const;

    ~Memory();
private:
    bool mem_initialized = false;
    uint8_t* mem = nullptr;
    uint32_t mem_size = 0x0;
};

#endif //NES_MEMORY_H
