#ifndef NES_DIRECTMEMORY
#define NES_DIRECTMEMORY


#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdint>

#include "Memory.h"
#include "Debug.h"


class DirectMemory : public Memory {
public:
    DirectMemory() = delete;
    explicit DirectMemory(const uint32_t size);

    void reset() override;
    uint8_t read(int64_t& cycles, uint16_t addr) override;
    void write(int64_t& cycles, uint16_t addr, uint8_t data) override;
    void loadROM(const char* filename) override;

    ~DirectMemory() override {};
};

#endif