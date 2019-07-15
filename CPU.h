//
// Created by niels on 7/2/19.
//

#ifndef NES_CPU_H
#define NES_CPU_H

#include <stdint-gcc.h>
#include "defines.h"

#define STACK_LOCATION 0x100

class Memory;

typedef enum {
    C = 0, // Carry
    Z = 1, // Zero
    I = 2, // IRQ disable
    D = 3, // Decimal mode
    B = 4, // BRK
    V = 6, // Overflow
    N = 7, // Negative
} status_flags;

typedef struct {
    uint16_t PC; // program counter
    uint8_t  AC; // accumulator
    uint8_t   X; // X register
    uint8_t   Y; // Y register
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
    bool flags[8];
    std::map<uint8_t, instruction> instructions;
    Memory* mem = nullptr;

    void executeInstruction(const instruction& instr);
    void update_CV_flags(uint8_t param, int16_t result);
    void update_ZN_flags(uint8_t param);
    void push_stack(uint8_t value);
    uint8_t pop_stack();

    // Helper functions
    uint8_t convertFlagsToByte();
    void loadFlagsFromByte(uint8_t byte);
    void printStatus();
};


#endif //NES_CPU_H
