#ifndef NES_MEMORY_H
#define NES_MEMORY_H


#include <stdint-gcc.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

#include "Debug.h"


class Memory {
public:
    Memory() = delete;
    Memory(const uint32_t size);

    void reset();
    uint8_t read(int64_t& cycles, uint16_t addr);
    void write(int64_t& cycles, uint16_t addr, uint8_t data);
    void loadROM(const char* filename);

    uint8_t* getMemoryPointer() { return mem; };
    const uint32_t getMemorySize() const { return (const uint32_t) mem_size; };

    virtual ~Memory() { free(mem); };

protected:
    bool mem_initialized = false;
    uint8_t* mem = nullptr;
    uint32_t mem_size = 0x0;
};

#endif //NES_MEMORY_H
