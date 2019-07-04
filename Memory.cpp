//
// Created by niels on 7/2/19.
//

#include "Memory.h"

Memory::Memory() {
    reset();
}

void Memory::loadBinary(const char *filename) {
    std::cout << "Loading binary program: " << filename << std::endl;
    std::ifstream file(filename, std::ios::binary);
    file.seekg(0, std::ios::beg);

    char buffer[MAX_MEM];
    if(!file.read(buffer, MAX_MEM)) {
        std::cout << "Something went wrong while reading file" << std::endl;
        file.close();
        return;
    }

    std::cout << "File loaded." << std::endl;
    memcpy(mem, buffer, MAX_MEM);
    file.close();
}

uint8_t Memory::get(uint16_t addr, addr_mode mode, cpu_state state) {
    switch(mode) {
        case ADDR_IMM:
            return addr; // value given directly used
        case ADDR_ZER:
            // Page 0: 0x0000 - 0x00FF, only 0xFF needed (save 2 bytes)
            if(addr < 0x0100)
                return mem[addr];
            else
                return 0x00;
        case ADDR_ZEX:
            if(addr + state.X < 0x0100)
                return mem[addr + state.X];
            else
                return 0x00;
        case ADDR_ZEY:
            if(addr + state.Y < 0x0100)
                return mem[addr + state.Y];
            else
                return 0x00;
        case ADDR_ABS:
            return mem[addr];
        case ADDR_ABX:
            return mem[addr + state.X];
        case ADDR_ABY:
            return mem[addr + state.Y];
        case ADDR_REL:
        case ADDR_IND:
        case ADDR_INX:
        case ADDR_INY:
            /*temp = 0x0000;
            temp = temp | mem[addr];
            temp = temp | (mem[addr + 1] << 8);
            return mem[temp];*/
            std::cout << "Indirect addressing and relative addressing should be handled in the CPU.";
            return 0x00;
        case ADDR_ACC:
        case ADDR_IMP:
        default:
            return 0x00;
    }
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
}