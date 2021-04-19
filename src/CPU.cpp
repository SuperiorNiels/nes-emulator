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

void CPU::reset() {
    state.PC = 0x0400;
    state.AC = 0x00;
    state.X = 0x00;
    state.Y = 0x00;
    state.SP = 0xFF;

    memset(&flags, 0b0, 8);
    flags[4] = true;
    flags[5] = true;
    printf("RESET\n");
    signals[RESET] = false;
}

void CPU::irq() {
    printf("IRQ\n");
    signals[IRQ] = false;
}

void CPU::nmi() {
    printf("NMI\n");
    signals[NMI] = false;
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
    while(cycles <= max_cycles) {
        if(state.PC == 0x336d) {
            printf("Reached end PC successfully with %d cycles (all tests succeeded!)\n", cycles);
            break;
        }

        // Check CPU flags
        if(signals[IRQ]) irq();
        if(signals[NMI]) nmi();
        if(signals[RESET]) reset();

        // get opcode
        uint8_t opcode = mem->read(cycles, state.PC);

        // execute instruction
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

void CPU::push_stack(int64_t& cycles, uint8_t value) {
    mem->write(cycles, STACK_LOCATION + state.SP--, value);
}

uint8_t CPU::pop_stack(int64_t& cycles) {
    state.SP++;
    return mem->read(cycles, STACK_LOCATION + state.SP);
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
        //if(i != 4 && i != 5)
            flags[i] = (byte & (1u << i)) != 0;
    }
}

void CPU::printStatus() {
    printf("\t[PC]: %4X [AC]: %2X [SP]: %4X [X]: %2X [Y]: %2X",
            state.PC, state.AC, state.SP, state.X, state.Y);
}

void CPU::printFlags() {
    printf("\tFlags: ");
    for(uint8_t i = 0; i < 8; i++) {
        if(flags[i]) printf("%c", flags_chr[i]);
        else printf("-");
    }
}

void CPU::printFullState() {
    int64_t tmp = 0; // use tmp cycles variable so the used cylces are incremented (not actually part of the prg) 
    uint8_t opcode = mem->read(tmp, state.PC);
    printf("Opcode: %s (%2X) ", instructions[opcode].name.c_str(), opcode);
    printStatus();
    printFlags();
    printf("\tCycles: %d", cycles);
    printf("\n");
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
            if(flags[D]) { printf("Decimal mode not supported!"); break; }
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
            result = state.PC + instr.bytes + 1;
            push_stack(cycles, result >> 8);
            push_stack(cycles, result);
            push_stack(cycles, convertFlagsToByte(true));
            state.PC = mem->read16(cycles, 0xFFFE);
            flags[B] = true;
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
            state.PC = (pop_stack(cycles) | (pop_stack(cycles) << 8));
            return;
        case RTS:
            state.PC = (pop_stack(cycles) | (pop_stack(cycles) << 8)) + 1;
            return;
        case SBC:
            if(flags[D]) { printf("Decimal mode not supported!"); break; }
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
            std::cout << "Unrecognized opcode!" << std::endl;
            //flags[B] = true;
            break;
    }

    state.PC += instr.bytes;
}