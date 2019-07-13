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

uint8_t Memory::get(addr_mode mode, cpu_state state) {
    uint16_t temp = 0x0000;

    switch(mode) {
        case ADDR_IMM:
            return mem[state.PC]; // value given directly used
        case ADDR_ZER:
            // Page 0: 0x0000 - 0x00FF, only 0xFF needed (save 2 bytes)
            temp = mem[state.PC];
            if(temp < 0x0100)
                return mem[temp];
            else {
                std::cout << "Zero page address to large!" << std::endl;
                return 0x00;
            }
        case ADDR_ZEX:
            temp = mem[state.PC];
            if(temp < 0x0100)
                return mem[(uint8_t) temp + state.X];
            else {
                std::cout << "Zero page address to large!" << std::endl;
                return 0x00;
            }
        case ADDR_ZEY:
            temp = mem[state.PC];
            if(temp < 0x0100)
                return mem[(uint8_t) temp + state.Y];
            else {
                std::cout << "Zero page address to large!" << std::endl;
                return 0x00;
            }
        case ADDR_ABS:
            temp = mem[state.PC] | (mem[state.PC + 1] << 8);
            return mem[temp];
        case ADDR_ABX:
            temp = mem[state.PC] | (mem[state.PC + 1] << 8);
            return mem[temp + state.X];
        case ADDR_ABY:
            temp = mem[state.PC] | (mem[state.PC + 1] << 8);
            return mem[temp + state.Y];
        case ADDR_REL:
            // addr is used as offset value
            temp = mem[state.PC];
            if(temp < 0x0100) {
                auto signed_offset = (int8_t) temp;
                return mem[state.PC + signed_offset];
            } else return 0x00;
        case ADDR_IND:
            temp = mem[state.PC] | (mem[state.PC + 1] << 8);
            temp = mem[temp] | (mem[temp + 1] << 8);
            return temp;
        case ADDR_INX:
            temp = mem[state.PC] + state.X;
            return mem[temp] | (mem[temp + 1] << 8);
        case ADDR_INY:
            temp = mem[state.PC] + state.Y;
            return mem[temp] | (mem[temp + 1] << 8);
        case ADDR_ACC:
        case ADDR_IMP:
        default:
            return 0x00;
    }
}

void Memory::set(uint16_t addr, uint8_t value) {
    mem[addr] = value;
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