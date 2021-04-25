#include "CPU.h"
#include "Memory.h"

CPU::CPU() {
    initialize_instructions(&instructions);
    // Set cpu signals
    signals.emplace(BREAK, false);
    signals.emplace(IRQ, false);
    signals.emplace(NMI, false);
    signals.emplace(RESET, false);
}

cpu_state CPU::getCPUState() const {
    return state;
}

bool* CPU::getCPUFlags() const {
    return (bool*) flags;
}

int64_t CPU::getCPUExecutedCycles() const {
    return cycles;
}

instruction CPU::getCurrentInstruction() {
    int64_t tmp;
    uint8_t opcode = mem->read(tmp, state.PC); 
    return instructions[opcode];
}

void CPU::reset() {
    state.PC = reset_vector;
    state.AC = 0x00;
    state.X = 0x00;
    state.Y = 0x00;
    state.SP = 0xFF;
    memset(&flags, 0b0, 8);
    flags[4] = true; // brk
    flags[5] = true; // unused (always 1)
    signals[RESET] = false;
    cycles += 7; // fake cycle count
}

void CPU::setResetVector(uint16_t vector) {
    reset_vector = vector;
}

void CPU::irq() {
    interrupt(0xFFFE);
    signals[IRQ] = false;
}

void CPU::nmi() {
    interrupt(0xFFFA);
    signals[NMI] = false;
}

void CPU::interrupt(uint16_t vector) {
    uint16_t result;
    result = state.PC + 2;
    push_stack(cycles, result >> 8);
    push_stack(cycles, result);
    push_stack(cycles, convertFlagsToByte());
    state.PC = mem->read16(cycles, vector);
    flags[I] = true;
}

bool const CPU::readCPUSignal(cpu_signals signal) { 
    return signals[signal]; 
}

void CPU::setCPUSignal(cpu_signals signal, bool state) {
    signals[signal] = state;
}

void CPU::attachMemeory(Memory* memory) {
    CPU::mem = memory;
}

void CPU::execute(int64_t max_cycles) {
    uint16_t prev_pc = 0;
    uint64_t start_cycles = cycles;
    while(cycles <= start_cycles + max_cycles) {
        // Check CPU signals
        if(signals[IRQ]) { irq(); continue; }
        if(signals[NMI]) { nmi(); continue; }
        if(signals[RESET]) { reset(); continue; }

        // get opcode
        uint8_t opcode = mem->read(cycles, state.PC);

        // execute instruction
        if(instructions.find(opcode) != instructions.end()) executeInstruction(cycles, instructions[opcode]);
        else DEBUG("Opcode %2X not found.\n", opcode);

        if(prev_pc == state.PC) break; // stop execution when stuck
        prev_pc = state.PC;
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

void CPU::push_stack(int64_t& cycles, uint8_t value) {
    mem->write(cycles, STACK_LOCATION + state.SP--, value);
}

uint8_t CPU::pop_stack(int64_t& cycles) {
    state.SP++;
    return mem->read(cycles, STACK_LOCATION + state.SP);
}

uint8_t CPU::convertFlagsToByte() {
    uint8_t res = 0;
    for(uint8_t i = 0; i < 8; i++) {
        if(i == 4) res |= (1u << i); 
        else if(i == 5) res |= (1u << i);
        else if(flags[i]) res |= (1u << i);
    }
    return res;
}

void CPU::loadFlagsFromByte(uint8_t byte) {
    for(uint8_t i = 0; i < 8; i++) {
        //if(i != 4 && i != 5)
            flags[i] = (byte & (1u << i)) != 0;
    }
}

void CPU::printStatus() {
    DEBUG("\t[PC]: %4X [AC]: %2X [SP]: %4X [X]: %2X [Y]: %2X",
            state.PC, state.AC, state.SP, state.X, state.Y);
}

void CPU::printFlags() {
    DEBUG("\tFlags: ");
    for(uint8_t i = 0; i < 8; i++) {
        if(flags[i]) DEBUG("%c", flags_chr[i]);
        else DEBUG("-");
    }
}

void CPU::printFullState() {
    int64_t tmp = 0; // use tmp cycles variable so the used cylces are incremented (not actually part of the prg) 
    uint8_t opcode = mem->read(tmp, state.PC);
    DEBUG("Opcode: %s (%2X) ", instructions[opcode].name, opcode);
    printStatus();
    printFlags();
    DEBUG("\tCycles: %ld", cycles);
    DEBUG("\n");
}

void CPU::executeInstruction(int64_t& cycles, const instruction& instr) {
    uint8_t param;
    uint16_t result, mem_loc;

    if(instr.mode != ADDR_IMP) {
        mem_loc = mem->calc_addr(cycles, instr.mode, state);
        param = mem->read(cycles, mem_loc);
    }

    cycles ++; // cycle for the execution of the instrcution
    switch(instr.type) {
        case ADC:
            if(flags[D]) { DEBUG("Decimal mode not supported!"); break; }
            result = (uint16_t) state.AC + (uint16_t) param + (uint16_t) flags[C];
            update_CV_flags(param, result);
            update_ZN_flags(result);
            state.AC = result;
            break;
        case AND:
            state.AC &= param;
            update_ZN_flags(state.AC);
            break;
        case ASL:
            if(instr.mode == ADDR_ACC) {
                flags[C] = state.AC & 0b10000000;
                state.AC = state.AC << 1u;
                update_ZN_flags(state.AC);
                break;
            }
            flags[C] = param & 0b10000000;
            param = param << 1u;
            mem->write(cycles, mem_loc, param);
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
            flags[Z] = result == 0x00;
            flags[V] = (param & (1u << 6));
            flags[N] = (param & (1u << 7));
            break;
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
            flags[B] = true;
            interrupt(0xFFFE);
            flags[I] = true;
            return;
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
                flags[C] = state.AC & 0b00000001;
                state.AC = state.AC >> 1u;
                update_ZN_flags(state.AC);
                break;
            }
            flags[C] = param & 0b00000001;
            param = param >> 1u;
            mem->write(cycles, mem_loc, param);
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
            push_stack(cycles, convertFlagsToByte());
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
            state.PC = (pop_stack(cycles) | (pop_stack(cycles) << 8));
            return;
        case RTS:
            state.PC = (pop_stack(cycles) | (pop_stack(cycles) << 8)) + 1;
            return;
        case SBC:
            if(flags[D]) { DEBUG("Decimal mode not supported!"); break; }
            param ^= 0xFF;
            result = (uint16_t) state.AC + (uint16_t) param + (uint16_t) flags[C];
            update_CV_flags(param, result);
            update_ZN_flags(result);
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
            DEBUG("Unrecognized opcode!\n");
            //flags[B] = true;
            break;
    }

    state.PC += instr.bytes;
}