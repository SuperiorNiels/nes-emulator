#ifndef NES_CPU_H
#define NES_CPU_H

#include <iostream>
#include <stdint-gcc.h>
#include "Instructions.h"
#include "Debug.h"

#define STACK_LOCATION 0x0100 // second page resevered for stack

class Memory;

typedef enum {
    BREAK = 0,
    IRQ = 1,
    NMI = 2,
    RESET = 3
} cpu_signals;

typedef enum {
    C = 0, // Carry
    Z = 1, // Zero
    I = 2, // IRQ disable
    D = 3, // Decimal mode
    B = 4, // BRK
    V = 6, // Overflow
    N = 7, // Negative
} status_flags;

const char flags_chr[8] = {'C', 'Z', 'I', 'D', 'B', ' ', 'V', 'N'}; 

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
    void execute(int64_t max_cycles);

    // Signal interactions
    bool const readCPUSignal(cpu_signals signal);
    void setCPUSignal(cpu_signals signal, bool state);
    void setResetVector(uint16_t vector);

    // Get CPU information for displaying
    cpu_state getCPUState() const;
    bool* getCPUFlags() const;
    int64_t getCPUExecutedCycles() const;
    instruction getCurrentInstruction();

private:
    bool flags[8];
    cpu_state state{};
    Memory* mem = nullptr;
    std::map<uint8_t, instruction> instructions;
    
    int64_t cycles = 0;
    std::map<const cpu_signals, bool> signals;
    uint16_t reset_vector = 0xFFFC;

    void nmi();
    void irq();
    void reset();
    void interrupt(uint16_t vector);
    void executeInstruction(int64_t& cycles, const instruction& instr);
    void update_CV_flags(uint8_t param, int16_t result);
    void update_ZN_flags(uint8_t param);
    void push_stack(int64_t& cycles, uint8_t value);
    uint8_t pop_stack(int64_t& cycles);

    // Helper functions
    uint8_t convertFlagsToByte();
    void loadFlagsFromByte(uint8_t byte);
    void printStatus();
    void printFlags();
    void printFullState();
};


#endif //NES_CPU_H
