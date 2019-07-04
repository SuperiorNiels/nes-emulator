//
// Created by niels on 7/2/19.
//

#include <iostream>
#include "CPU.h"
#include "Memory.h"

CPU::CPU() {
    state.PC = 0x0000;
    state.AC = 0x00;
    state.X = 0x00;
    state.Y = 0x00;
    state.SR = 0x00;
    state.SP = 0x00;

    initialize_instructions(&instructions);
    std::cout << "temp";
}

void CPU::attachMemeory(Memory* memory) {
    CPU::mem = memory;
}

void CPU::setProgramCounter(uint16_t pc) {
    state.PC = pc;
}

void CPU::execute() {}

void CPU::executeInstruction(instruction instr) {}