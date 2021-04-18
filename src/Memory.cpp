#include "Memory.h"

Memory::Memory() {
    reset();
}

void Memory::loadBinary(const char *filename) {
    std::cout << "Loading binary program: " << filename << std::endl;
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::cout << "File size: " << size << std::endl;

    if(size > MAX_MEM + 1) {
        std::cout << "File is to big (max. 64K)." << std::endl;
        return;
    }

    char buffer[MAX_MEM + 1];
    if(!file.read(buffer, size)) {
        std::cout << "Something went wrong while reading file" << std::endl;
        file.close();
        return;
    }

    std::cout << "File loaded." << std::endl;
    memcpy(mem, buffer, MAX_MEM + 1);
    file.close();
}

uint16_t Memory::calc_addr(int32_t& cycles, addr_mode mode, cpu_state state) {
    uint16_t temp;

    switch(mode) {
        case ADDR_IMP:
        case ADDR_ACC:
            break;
        case ADDR_IMM:
            return state.PC + 1;
        case ADDR_ZER:
            return mem[state.PC + 1];
        case ADDR_ZEX:
            return mem[state.PC++] + state.X;
        case ADDR_ZEY:
            return mem[state.PC++] + state.Y;
        case ADDR_REL:
            //temp = (int8_t) mem[state.PC + 1];
            //return mem[state.PC + temp];
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
            temp = (read16(cycles, state.PC + 1) + state.X) % 0xFF;
            return mem[temp] | (mem[temp + 1] << 8);
        case ADDR_INY:
            temp = read16(cycles, state.PC + 1);
            return mem[temp] | (mem[temp + 1] << 8) + state.Y;
        default:
            std::cout << "ERROR: wrong addressing mode" << std::endl;
            break;
    }
    return 0x0000;
}

uint8_t Memory::read(int32_t& cycles,uint16_t addr) {
    uint8_t ppu_register;
    cycles--; // alyways use 1 cycle for reading
    /*switch(addr) {
        case 0 ... 0x07FF: return mem[addr];        // 2kB internal RAM
        case 0x0800 ... 0x0FFF: return mem[addr - 0x0800];   // mirror of RAM
        case 0x1000 ... 0x17FF: return mem[addr - 0x1000];   // mirror of RAM
        case 0x1800 ... 0x1FFF: return mem[addr - 0x1800];   // mirror of RAM
        case 0x2000 ... 0x3FFF:
            // PPU registers: 0x2000 - 0x2007, rest are mirror of these 8 registers
            ppu_register = (addr - 0x2000) % 8;
            break;
        case 0x4000 ... 0x4017: break; // NES APU and I/O registers
        case 0x4018 ... 0x401F: break; // normally disabled
        case 0x4020 ... 0xFFFF:
            // Read from cartridge
            break;
        default:
            break;
    }*/
    return mem[addr];
    //return 0x00;
}

uint16_t Memory::read16(int32_t& cycles, uint16_t addr) {
    cycles--;
    cycles--;
    return (mem[addr] | (mem[addr + 1] << 8));
}

void Memory::write(int32_t& cycles, uint16_t addr, uint8_t data) {
    cycles--;
    mem[addr] = data;
}

void Memory::reset() {
    memset(mem, 0, 0xFFFF);
}

void Memory::dump() {
    uint8_t count = 1;
    for(unsigned char i : mem) {
        printf("%.2X ", i);
        if(count == 8) {
            printf("\n");
            count = 0;
        }
        count++;
    }
    printf("\n");
}