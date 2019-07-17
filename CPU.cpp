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
    while(!flags[B]) {
        uint8_t opcode = mem->read(state.PC);
        executeInstruction(instructions[opcode]);
        printStatus();
    }
}

void CPU::update_CV_flags(uint8_t param, int16_t result) {
    flags[C] = result > 0xFF;
    flags[V] = ~(state.AC ^ param) & (state.AC ^ result) & 0x80;
}

void CPU::update_ZN_flags(uint8_t param) {
    flags[Z] = param == 0;
    flags[N] = param & 0x80;
}

void CPU::push_stack(uint8_t value) {                   // TODO:: PC counter is 16b
    mem->write(STACK_LOCATION + state.SP++, value);
}

uint8_t CPU::pop_stack() {
    state.SP--;
    return mem->read(STACK_LOCATION + state.SP);
}

void CPU::executeInstruction(const instruction& instr) {
    uint8_t param;
    uint16_t result, mem_loc, temp;

    mem_loc = mem->calc_addr(instr.mode, state);
    param = mem->read(mem_loc);
    std::cout << "Operation: " << instr.name << std::endl;
    printf("Param: %.2X\n", param);

    switch(instr.type) {
        case ADC:
            result = state.AC + param + (uint8_t) flags[C];
            update_CV_flags(param, result);
            update_ZN_flags(state.AC);
            state.AC = result;
            break;
        case AND:
            state.AC &= param;
            update_ZN_flags(state.AC);
            break;
        case ASL:
            if(instr.mode == ADDR_ACC) {
                flags[C] = state.AC & 0x40;
                state.AC = state.AC << 1u;
                update_ZN_flags(state.AC);
                break;
            }
            flags[C] = param & 0x40;
            mem->write(mem_loc, param << 1u);
            update_ZN_flags(param);
            break;
        case BCC:
            if(!flags[C]) { state.PC += (int8_t) param; return; }
            break;
        case BCS:
            if(flags[C]) { state.PC += (int8_t) param + instr.bytes; return; }
            break;
        case BEQ:
            if(flags[Z]) { state.PC += (int8_t) param + instr.bytes; return; }
            break;
        case BIT:
            result = state.AC & param;
            if(result == 0) flags[Z] = true;
            flags[V] = (result & (1u << 6)) != 0;
            flags[N] = (result & (1u << 7)) != 0;
        case BMI:
            if(flags[N]) { state.PC += (int8_t) param + instr.bytes; return; }
            break;
        case BNE:
            if(!flags[Z]) { state.PC += (int8_t) param + instr.bytes; return; }
            break;
        case BPL:
            if(!flags[N]) { state.PC += (int8_t) param + instr.bytes; return; }
            break;
        case BRK:
            push_stack(state.PC >> 8);
            push_stack(state.PC);
            push_stack(convertFlagsToByte());
            state.PC = mem->read16(0xFFFE);
            flags[B] = true;
            break;
        case BVC:
            if(!flags[V]) { state.PC += (int8_t) param + instr.bytes; return; }
            break;
        case BVS:
            if(flags[V]) { state.PC += (int8_t) param + instr.bytes; return; }
            break;
        case CLC:
            flags[C] = false;
            break;
        case CLD:
            flags[D] = false;
            break;
        case CLI:
            flags[I] = false;
            break;
        case CLV:
            flags[V] = false;
            break;
        case CMP:
            flags[C] = state.AC >= param;
            update_ZN_flags(state.AC - param);
            break;
        case CPX:
            flags[C] = state.X >= param;
            update_ZN_flags(state.X - param);
            break;
        case CPY:
            flags[C] = state.Y >= param;
            update_ZN_flags(state.Y - param);
            break;
        case DEC:
            mem->write(mem_loc, param - 1);
            update_ZN_flags(param - 1);
            break;
        case DEX:
            state.X--;
            update_ZN_flags(state.X);
            break;
        case DEY:
            state.Y--;
            update_ZN_flags(state.Y);
            break;
        case EOR:
            state.AC ^= param;
            update_ZN_flags(state.AC);
            break;
        case INC:
            mem->write(mem_loc, param + 1);
            update_ZN_flags(param + 1);
            break;
        case INX:
            state.X++;
            update_ZN_flags(state.X);
            break;
        case INY:
            state.Y++;
            update_ZN_flags(state.Y);
            break;
        case JMP:
            state.PC = mem_loc; // TODO: implement 6502 bug on page boundry?
            return;
        case JSR:
            push_stack(state.PC >> 8);
            push_stack(state.PC);
            state.PC = mem_loc;
            return;
        case LDA:
            state.AC = param;
            update_ZN_flags(state.AC);
            break;
        case LDX:
            state.X = param;
            printf("%.2X\n", mem_loc);
            update_ZN_flags(state.X);
            break;
        case LDY:
            state.Y = param;
            update_ZN_flags(state.Y);
            break;
        case LSR:
            if(instr.mode == ADDR_ACC) {
                flags[C] = (state.AC & 0x01) != 0;
                state.AC = state.AC >> 1u;
                update_ZN_flags(state.AC);
                break;
            }
            flags[C] = (param & 0x01) != 0;
            mem->write(mem_loc, param >> 1u);
            update_ZN_flags(param);
            break;
        case NOP:
            break;
        case ORA:
            state.AC |= param;
            update_ZN_flags(state.AC);
            break;
        case PHA:
            push_stack(state.AC);
            break;
        case PHP:
            push_stack(convertFlagsToByte());
            break;
        case PLA:
            state.AC = pop_stack();
            update_ZN_flags(state.AC);
            break;
        case PLP:
            loadFlagsFromByte(pop_stack());
            break;
        case ROL:
        case ROR:
        case RTI:
        case RTS:
            state.PC = ((pop_stack() << 8) | pop_stack()) + 3;
            return;
        case SBC:

        case SEC:
            flags[C] = true;
            break;
        case SED:
            flags[D] = true;
            break;
        case SEI:
            flags[I] = true;
            break;
        case STA:
            mem->write(mem_loc, state.AC);
            break;
        case STX:
            mem->write(mem_loc, state.X);
            break;
        case STY:
            mem->write(mem_loc, state.Y);
            break;
        case TAX:
            state.X = state.AC;
            update_ZN_flags(state.X);
            break;
        case TAY:
            state.Y = state.AC;
            update_ZN_flags(state.Y);
            break;
        case TSX:
            state.X = state.SP;
            update_ZN_flags(state.X);
            break;
        case TXA:
            state.AC = state.X;
            update_ZN_flags(state.AC);
            break;
        case TXS:
            state.SP = state.X;
            break;
        case TYA:
            state.AC = state.Y;
            update_ZN_flags(state.AC);
            break;
        default:
            std::cout << "Unrecognized opcode, HALT!" << std::endl;
            flags[B] = true;
            break;
    }

    state.PC += instr.bytes;
}

uint8_t CPU::convertFlagsToByte() {
    uint8_t res = 0;
    for(uint8_t i = 0; i < 8; i++)
        if(flags[i]) res |= (1u << i);
    return res;
}

void CPU::loadFlagsFromByte(uint8_t byte) {
    for(uint8_t i = 0; i < 8; i++) {
        flags[i] = (byte & (1u << i)) != 0;
    }
}

void CPU::printStatus() {
    printf("------ CPU State dump ------\n");
    printf("PC: %.2X\n", state.PC);
    printf("AC: %.2X\n", state.AC);
    printf("X: %.2X\n", state.X);
    printf("Y: %.2X\n", state.Y);
}