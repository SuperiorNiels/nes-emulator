//
// Created by niels on 7/2/19.
//

#ifndef NES_CPU_H
#define NES_CPU_H

#include <stdint-gcc.h>
#include "defines.h"

class Memory;

typedef struct {
    uint16_t PC; // program counter
    uint8_t  AC; // accumulator
    uint8_t   X; // X register
    uint8_t   Y; // Y register
    uint8_t  SR; // status register
    uint8_t  SP; // stack pointer
} cpu_state;

class CPU {
public:
    CPU();

    void attachMemeory(Memory* memory);
    void setProgramCounter(uint16_t pc);
    void execute();
private:
    cpu_state state{};
    std::map<uint8_t, instruction> instructions;
    Memory* mem = nullptr;

    void executeInstruction(instruction instr);
};


#endif //NES_CPU_H
