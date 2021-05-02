#ifndef NES_MEMORY_H
#define NES_MEMORY_H


#include <stdint-gcc.h>
#include <cstdlib>


class Memory {
public:
    Memory() {};

    virtual void reset() = 0;
    virtual uint8_t read(int64_t& cycles, uint16_t addr) = 0;
    virtual void write(int64_t& cycles, uint16_t addr, uint8_t data) = 0;
    virtual void loadROM(const char* filename) = 0;

    uint8_t* getMemoryPointer() { return mem; };
    const uint32_t getMemorySize() const { return (const uint32_t) mem_size; };

    virtual ~Memory() { free(mem); };

protected:
    bool mem_initialized = false;
    uint8_t* mem = nullptr;
    uint32_t mem_size = 0x0;
};

#endif //NES_MEMORY_H
