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

uint8_t Memory::get(uint16_t addr) {
    return (uint8_t) mem[addr];
}

void Memory::write(uint16_t addr, uint8_t data) {
    mem[addr] = data;
}

void Memory::reset() {
    memset(mem, 0, 0xFFFF);
}