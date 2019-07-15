//
// Created by niels on 7/2/19.
//

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

    char buffer[MAX_MEM];
    if(!file.read(buffer, size)) {
        std::cout << "Something went wrong while reading file" << std::endl;
        file.close();
        return;
    }

    std::cout << "File loaded." << std::endl;
    memcpy(mem, buffer, MAX_MEM);
    file.close();
}

uint16_t Memory::calc_addr(addr_mode mode, cpu_state state) {
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
            return read16(state.PC + 1);
        case ADDR_ABX:
            return read16(state.PC + 1) + state.X;
        case ADDR_ABY:
            return read16(state.PC + 1) + state.Y;
        case ADDR_IND:
            temp = read16(state.PC + 1);
            return mem[temp] | (mem[temp + 1] << 8);
        case ADDR_INX:
            temp = (read16(state.PC + 1) + state.X) % 0xFF;
            return mem[temp] | (mem[temp + 1] << 8);
        case ADDR_INY:
            temp = read16(state.PC + 1);
            return mem[temp] | (mem[temp + 1] << 8) + state.Y;
        default:
            std::cout << "ERROR: wrong addressing mode" << std::endl;
            break;
    }
    return 0x0000;
}

uint8_t Memory::read(uint16_t addr) {
    return mem[addr];
}

uint16_t Memory::read16(uint16_t addr) {
    return (mem[addr] | (mem[addr + 1] << 8));
}

void Memory::write(uint16_t addr, uint8_t data) {
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