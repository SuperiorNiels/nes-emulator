#include "Memory.h"


Memory::Memory(const uint32_t size) {
    this->mem_size = size;
    this->mem = new uint8_t[(int) size]{};
    this->mem_initialized = true;
}

bool Memory::openROM(const char* filename) {
    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return false; } 
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
        return false;
    }

    DEBUG("File loaded.\n");
    memcpy(mem, buffer, mem_size + 1);
    file.close();
    return true;
}

uint8_t Memory::read(int64_t& cycles, uint16_t addr) {
    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return 0x00; } 
    cycles++; // alyways use 1 cycle for reading
    return mem[addr];
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