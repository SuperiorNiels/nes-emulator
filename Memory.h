//
// Created by niels on 7/2/19.
//

#ifndef NES_MEMORY_H
#define NES_MEMORY_H

#include <cstring>
#include <fstream>
#include <iostream>
#include <cstdint>

# define MAX_MEM 0xFFFF

class Memory {
public:
    Memory();

    void loadBinary(const char* filename);
    uint8_t get(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

    void reset();

private:
    uint8_t mem[MAX_MEM]; // 16K
};


#endif //NES_MEMORY_H
