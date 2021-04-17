#ifndef NES_CPU_H
#define NES_CPU_H

#include <stdint-gcc.h>
#include "Instructions.h"

#define STACK_LOCATION 0x0100 // second page resevered for stack

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
    void execute(int32_t& cycles);
    void reset();
private:
    cpu_state state{};
    bool flags[8];
    std::map<uint8_t, instruction> instructions;
    Memory* mem = nullptr;

    void executeInstruction(int32_t& cycles, const instruction& instr);
    void update_CV_flags(uint8_t param, int16_t result);
    void update_ZN_flags(uint8_t param);
    void push_stack(int32_t& cycles, uint8_t value);
    uint8_t pop_stack(int32_t& cycles);

    // Helper functions
    uint8_t convertFlagsToByte(bool brk_flag);
    void loadFlagsFromByte(uint8_t byte);
    void printStatus();
    void printFlags();
};


#endif //NES_CPU_H
