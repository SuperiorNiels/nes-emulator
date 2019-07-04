//
// Created by niels on 7/2/19.
//

#ifndef NES_CPU_H
#define NES_CPU_H


#include <stdint-gcc.h>

typedef enum {
    IMP = 1,
    ACC = 2,
    IMM = 3,
    ZER = 4,
    ZEX = 5,
    ZEY = 6,
    REL = 7,
    ABS = 8,
    ABX = 9,
    ABY = 10,
    IND = 11,
    INX = 12,
    INY = 13,
} addr_mode;

typedef struct {
    addr_mode mode;
    uint8_t opcode;
} instrucion;

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
