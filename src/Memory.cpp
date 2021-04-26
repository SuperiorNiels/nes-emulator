#include "Memory.h"


Memory::Memory(const uint32_t mem_size) {
    this->mem_size = mem_size;
    this->mem = new uint8_t[(int) mem_size]{};
    this->mem_initialized = true;
}

uint8_t* Memory::getMemoryStartPointer() {
    return mem;
}

const uint32_t Memory::getMemorySize() const {
    return (const uint32_t) mem_size;
}

void Memory::loadBinary(const char *filename) {
    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return; } 
    DEBUG("Loading binary program: %s\n", filename);
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    DEBUG("File size: %ld\n", size);

    if(size > mem_size + 1) {
        DEBUG("File is to big (max. 64K).\n");
        //return;
        size = mem_size + 1; // only copy the max size
    }

    char buffer[mem_size + 1];
    if(!file.read(buffer, size)) {
        DEBUG("Something went wrong while reading file.\n");
        file.close();
        return;
    }

    DEBUG("File loaded.\n");
    memcpy(mem, buffer, mem_size + 1);
    file.close();
}

uint16_t Memory::calc_addr(int64_t& cycles, addr_mode mode, const cpu_state& state) {    
    uint16_t temp;

    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return 0x0000; } 

    switch(mode) {
        case ADDR_IMP:
        case ADDR_ACC:
            break;
        case ADDR_IMM:
            return state.PC + 1;
        case ADDR_ZER:
            return read(cycles, state.PC + 1);
        case ADDR_ZEX:
            return (read(cycles, state.PC + 1) + state.X) & 0x00FF;
        case ADDR_ZEY:
            return (read(cycles, state.PC + 1) + state.Y) & 0x00FF;
        case ADDR_REL:
            return state.PC + 1;
        case ADDR_ABS:
            return read16(cycles, state.PC + 1);
        case ADDR_ABX:
            return read16(cycles, state.PC + 1) + state.X;
        case ADDR_ABY:
            return read16(cycles, state.PC + 1) + state.Y;
        case ADDR_IND:
            temp = read16(cycles, state.PC + 1);
            return mem[temp] | (mem[temp + 1] << 8);
        case ADDR_INX:
            temp = (read16(cycles, state.PC + 1) + state.X) & 0xFF;
            return read16(cycles, temp);
        case ADDR_INY:
            temp = read16(cycles, read(cycles, state.PC + 1));
            return temp + state.Y;
        default:
            DEBUG("ERROR: unkown addressing mode.\n");;
            break;
    }
    return 0x0000;
}

uint8_t Memory::read(int64_t& cycles,uint16_t addr) {
    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return 0x00; } 
    uint8_t ppu_register;
    cycles++; // alyways use 1 cycle for reading
    return mem[addr];
}

uint16_t Memory::read16(int64_t& cycles, uint16_t addr) {
    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return 0x0000; } 
    cycles++;
    cycles++;
    return (mem[addr] | (mem[addr + 1] << 8));
}

void Memory::write(int64_t& cycles, uint16_t addr, uint8_t data) {
    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return; } 
    cycles++;
    mem[addr] = data;
}

void Memory::reset() {
    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return; } 
    memset(mem, 0, mem_size);
}

Memory::~Memory() {
    delete mem;
}