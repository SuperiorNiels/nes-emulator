#ifndef NES_NESMEMORY
#define NES_NESMEMORY


#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdint>

#include "Memory.h"
#include "Debug.h"


class NESMemory : public Memory {
public:
    NESMemory();

    void reset() override;
    uint8_t read(int64_t& cycles, uint16_t addr) override;
    void write(int64_t& cycles, uint16_t addr, uint8_t data) override;
    void loadROM(const char* filename) override;

    ~NESMemory() override {};

private:
};

#endif