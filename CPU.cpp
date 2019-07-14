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
    state.SP = 0x00;

    memset(&flags, 0, 8);

    initialize_instructions(&instructions);
}

void CPU::attachMemeory(Memory* memory) {
    CPU::mem = memory;
}

void CPU::setProgramCounter(uint16_t pc) {
    state.PC = pc;
}

void CPU::execute() {
    state.PC = 0x0000;
    while(running) {
        uint8_t temp = mem->get(ADDR_IMM, state);
        executeInstruction(instructions[temp]);
        printStatus();
    }
}

void CPU::update_CV_flags(uint8_t param, int16_t result) {
    flags[C] = result > 0xFF;
    flags[V] = ~(state.AC ^ param) & (state.AC ^ result) & 0x80;
}

void CPU::update_ZN_flags() {
    flags[Z] = state.AC == 0;
    flags[N] = state.AC & 0x80;
}

void CPU::executeInstruction(const instruction& instr) {
    uint8_t param;
    uint16_t result;

    state.PC++;
    param = mem->get(instr.mode, state);
    switch(instr.type) {
        case ADC:
            result = state.AC + param + (uint8_t) flags[C];
            update_CV_flags(param, result);
            update_ZN_flags();
            state.AC = result;
            break;
        case AND:
        case ASL:
        case BCC:
        case BCS:
        case BEQ:
        case BIT:
        case BMI:
        case BNE:
        case BPL:
        case BRK:
            std::cout << "Program done!" << std::endl;
            running = false;
            break;
        case BVC:
        case BVS:
        case CLC:
        case CLD:
        case CLI:
        case CLV:
        case CMP:
        case CPX:
        case CPY:
        case DEC:
        case DEX:
        case DEY:
        case EOR:
        case INC:
        case INX:
        case INY:
        case JMP:
        case JSR:
        case LDA:
            state.AC = mem->get(instr.mode, state);
            update_ZN_flags();
            break;
        case LDX:
            state.X = mem->get(instr.mode, state);
            update_ZN_flags();
            break;
        case LDY:
            state.Y = mem->get(instr.mode, state);
            update_ZN_flags();
            break;
        case LSR:
        case NOP:
        case ORA:
        case PHA:
        case PHP:
        case PLA:
        case PLP:
        case ROL:
        case ROR:
        case RTI:
        case RTS:
        case SBC:
        case SEC:
        case SED:
        case SEI:
        case STA:
            // TODO: split memory get function: get address (mode) and get content (byte, param)
        case STX:
        case STY:
        case TAX:
        case TAY:
        case TSX:
        case TXA:
        case TXS:
        case TYA:
            std::cout << instr.name << std::endl;
            break;
        default:
            return;
    }

    state.PC += instr.bytes - 1;
}

void CPU::printStatus() {
    printf("------ CPU State dump ------\n");
    printf("PC: %.2X\n", state.PC);
    printf("AC: %.2X\n", state.AC);
    printf("X: %.2X\n", state.X);
    printf("Y: %.2X\n", state.Y);
}