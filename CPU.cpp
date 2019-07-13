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
}

void CPU::attachMemeory(Memory* memory) {
    CPU::mem = memory;
}

void CPU::setProgramCounter(uint16_t pc) {
    state.PC = pc;
}

bool CPU::checkFlag(status_flags flag) {
    return state.SR & (1u << flag);
}

void CPU::setFlag(status_flags flag) {
    uint8_t temp = (1u << flag);
    state.SR |= temp;
}

void CPU::removeFlag(status_flags flag) {
    state.SR &= ~(1u << flag);
}

void CPU::toggleFlag(status_flags flag) {
    state.SR ^= (1u << flag);
}

void CPU::execute() {
    running = true;
    state.PC = 0x0000;
    while(running) {
        uint8_t temp = mem->get(ADDR_IMM, state);
        printf("Opcode: %.2X\n", temp);
        executeInstruction(instructions[temp]);
        printStatus();
    }
}

void CPU::executeInstruction(const instruction& instr) {
    uint8_t temp;
    uint16_t result;

    state.PC++;

    switch(instr.type) {
        case ADC:
            temp = mem->get(instr.mode, state);
            result = state.AC + temp + (uint8_t) checkFlag(CARRY);
            if(result > 0xFF00) {
                setFlag(CARRY);
                result = 0xFF;
            }
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
            if(state.AC == 0) setFlag(ZERO);
            if(state.AC & (1u << 7)) setFlag(NEGATIVE);
            break;
        case LDX:
        case LDY:
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
    printf("PC: %.2X\n", state.PC);
    printf("AC: %.2X\n", state.AC);
}