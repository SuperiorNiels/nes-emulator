#include <iostream>
#include "CPU.h"
#include "Memory.h"

CPU::CPU() {
    initialize_instructions(&instructions);
}

void CPU::reset() {
    state.PC = 0x0400;
    state.AC = 0x00;
    state.X = 0x00;
    state.Y = 0x00;
    state.SP = 0xFF;

    memset(&flags, 0b0, 8);
    flags[5] = true;
    if (mem != nullptr) {
        mem->reset();
    }
}

void CPU::attachMemeory(Memory* memory) {
    CPU::mem = memory;
}

void CPU::execute(int32_t& cycles) {
    bool debug = false;
    uint16_t prev_pc = 0;
    while(cycles > 0) {
        // debug when stuck
        if(state.PC == prev_pc) debug = true;
        prev_pc = state.PC;
        // debug specific mem address
        if(state.PC == 0x09c7) debug = true;
        // get opcode
        uint8_t opcode = mem->read(cycles, state.PC);
        if(debug) {
            //uint16_t mem_loc = 0x01fe;
            //printf("Mem[%4x]: %2x\n", mem_loc, mem->read(cycles, mem_loc));
            //debugPrintStack(cycles);
            printf("Current PC: %2x, Opcode: %s (%2X)\n", state.PC, instructions[opcode].name.c_str(), opcode);
            //printStatus();
            //printFlags();
        }
        if(instructions.find(opcode) != instructions.end()) executeInstruction(cycles, instructions[opcode]);
        else printf("Opcode %2X not found.\n", opcode);
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

void CPU::push_stack(int32_t& cycles, uint8_t value) {
    mem->write(cycles, STACK_LOCATION + state.SP--, value);
}

uint8_t CPU::pop_stack(int32_t& cycles) {
    state.SP++;
    return mem->read(cycles, STACK_LOCATION + state.SP);
}

void CPU::executeInstruction(int32_t& cycles, const instruction& instr) {
    uint8_t param;
    uint16_t result, mem_loc;

    mem_loc = mem->calc_addr(cycles, instr.mode, state);
    param = mem->read(cycles, mem_loc);

    //std::cout << "\nOperation: " <<  instr.name << std::endl;
    cycles--; // the actual operation cycle
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
                flags[C] = state.AC & 0x80;
                state.AC = state.AC << 1u;
                update_ZN_flags(state.AC);
                break;
            }
            flags[C] = param & 0x80;
            mem->write(cycles, mem_loc, param << 1u);
            update_ZN_flags(param);
            break;
        case BCC:
            if(!flags[C]) { state.PC += (int8_t) param + instr.bytes; return; }
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
            result = state.PC + instr.bytes - 1;
            push_stack(cycles, result >> 8);
            push_stack(cycles, result);
            push_stack(cycles, convertFlagsToByte(true));
            state.PC = mem->read16(cycles, 0xFFFE);
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
            mem->write(cycles, mem_loc, param - 1);
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
            mem->write(cycles, mem_loc, param + 1);
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
            result = state.PC + instr.bytes - 1;
            push_stack(cycles, result >> 8);
            push_stack(cycles, result);
            state.PC = mem_loc;
            return;
        case LDA:
            state.AC = param;
            update_ZN_flags(state.AC);
            break;
        case LDX:
            state.X = param;
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
            mem->write(cycles, mem_loc, param >> 1u);
            update_ZN_flags(param);
            break;
        case NOP:
            break;
        case ORA:
            state.AC |= param;
            update_ZN_flags(state.AC);
            break;
        case PHA:
            push_stack(cycles, state.AC);
            break;
        case PHP:
            push_stack(cycles, convertFlagsToByte(true));
            break;
        case PLA:
            state.AC = pop_stack(cycles);
            update_ZN_flags(state.AC);
            break;
        case PLP:
            loadFlagsFromByte(pop_stack(cycles));
            break;
        case ROL:
            if(instr.mode == ADDR_ACC) {
                result = flags[C];
                flags[C] = state.AC & 0x80;
                state.AC = (state.AC << 1u) | result;
                update_ZN_flags(state.AC);
            } else {
                result = flags[C];
                flags[C] = param & 0x80;
                param = (param << 1u) | result;
                update_ZN_flags(param);
                mem->write(cycles, mem_loc, param);
            }
            break;
        case ROR:
            if(instr.mode == ADDR_ACC) {
                result = flags[C] << 7;
                flags[C] = state.AC & 0x01;
                state.AC = result | (state.AC >> 1);
                update_ZN_flags(state.AC);
            } else {
                result = flags[C] << 7;
                flags[C] = param & 0x01;
                param = result | (param >> 1);
                update_ZN_flags(param);
                mem->write(cycles, mem_loc, param);
            }
            break;
        case RTI:
            loadFlagsFromByte(pop_stack(cycles));
            state.PC = (pop_stack(cycles) | (pop_stack(cycles) << 8)) + 1;
            return;
        case RTS:
            state.PC = (pop_stack(cycles) | (pop_stack(cycles) << 8)) + 1;
            return;
        case SBC:
            param ^= 0xFF;
            result = state.AC + param + (uint8_t) flags[C];
            update_CV_flags(param, result);
            update_ZN_flags(state.AC);
            state.AC = result;
            break;
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
            mem->write(cycles, mem_loc, state.AC);
            break;
        case STX:
            mem->write(cycles, mem_loc, state.X);
            break;
        case STY:
            mem->write(cycles, mem_loc, state.Y);
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
            std::cout << "Unrecognized opcode!" << std::endl;
            //flags[B] = true;
            break;
    }

    state.PC += instr.bytes;
}

uint8_t CPU::convertFlagsToByte(bool brk_flag) {
    uint8_t res = 0;
    for(uint8_t i = 0; i < 8; i++) {
        if(i == 4 && brk_flag) res |= (1u << i); 
        else if(i == 5) res |= (1u << i);
        else if(flags[i]) res |= (1u << i);
    }
    return res;
}

void CPU::loadFlagsFromByte(uint8_t byte) {
    for(uint8_t i = 0; i < 8; i++) {
        if(i != 4 && i != 5)
            flags[i] = (byte & (1u << i)) != 0;
    }
}

void CPU::printStatus() {
    printf("------ CPU State dump ------\n");
    printf("PC: %.2X\n", state.PC);
    printf("AC: %.2X\n", state.AC);
    printf("SP: %.2X\n", state.SP);
    printf("X: %.2X\n", state.X);
    printf("Y: %.2X\n", state.Y);
}

void CPU::printFlags() {
    printf("Flags:\n");
    for(uint8_t i = 0; i < 8; i++) {
        printf("%d", flags[i]);
    }
    printf("\nCZIDB-VN\n");
}

void CPU::debugPrintStack(int32_t& cycles) {
    uint8_t current_sp = state.SP;
    printf("Current stack (start: %4x)\n", STACK_LOCATION + state.SP);
    do {
        uint16_t loc = STACK_LOCATION + current_sp;
        printf("[%4x]: %2x\n", loc, mem->read(cycles, loc));
        current_sp++;
    } while(current_sp != 0);
}