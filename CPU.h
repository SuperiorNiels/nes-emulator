//
// Created by niels on 7/2/19.
//

#ifndef NES_CPU_H
#define NES_CPU_H


#include <stdint-gcc.h>

class CPU {
public:
    CPU();

private:
    uint16_t PC; // program counter
    uint8_t  AC; // accumulator
    uint8_t   X; // X register
    uint8_t   Y; // Y register
    uint8_t  SR; // status register
    uint8_t  SP; // stack pointer
};


#endif //NES_CPU_H
