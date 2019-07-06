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

void CPU::execute() {
    for(uint16_t i = 0; i < 0xFFFF; i++) {
        executeInstruction(instructions[mem->get(i, ADDR_ABS, state)]);
    }
}

void CPU::executeInstruction(instruction instr) {
    switch(instr.type) {
        case ADC:
            std::cout << "ADC" << std::endl;
            break;
        case AND:break;
        case ASL:break;
        case BCC:break;
        case BCS:break;
        case BEQ:break;
        case BIT:break;
        case BMI:break;
        case BNE:break;
        case BPL:break;
        case BRK:break;
        case BVC:break;
        case BVS:break;
        case CLC:break;
        case CLD:break;
        case CLI:break;
        case CLV:break;
        case CMP:break;
        case CPX:break;
        case CPY:break;
        case DEC:break;
        case DEX:break;
        case DEY:break;
        case EOR:break;
        case INC:break;
        case INX:break;
        case INY:break;
        case JMP:break;
        case JSR:break;
        case LDA:break;
        case LDX:break;
        case LDY:break;
        case LSR:break;
        case NOP:break;
        case ORA:break;
        case PHA:break;
        case PHP:break;
        case PLA:break;
        case PLP:break;
        case ROL:break;
        case ROR:break;
        case RTI:break;
        case RTS:break;
        case SBC:break;
        case SEC:break;
        case SED:break;
        case SEI:break;
        case STA:break;
        case STX:break;
        case STY:break;
        case TAX:break;
        case TAY:break;
        case TSX:break;
        case TXA:break;
        case TXS:break;
        case TYA:break;
        default:
            std::cout << "Unknown instruction type." << std::endl;
    }
}