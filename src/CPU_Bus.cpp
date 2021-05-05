#include "CPU_Bus.h"


CPU_Bus::CPU_Bus(Cartridge* cartridge) {
    this->cartridge = cartridge;
    this->cpuRAM = new Memory(2048); // 2Kb
    this->PPU_registers = new Memory(8); // 8 byte
    this->workRAM = new Memory(0x2000);
}

bool CPU_Bus::openROM(const char* filename) {
    return cartridge->openROM(filename);
}

uint8_t CPU_Bus::read(int64_t& cycles, uint16_t addr) {

    switch(addr) {
        case 0 ... 0x1FFF: return cpuRAM->read(cycles, addr & 0x07FF);                                                  // 2kB internal RAM
        case 0x2000 ... 0x3FFF: return PPU_registers->read(cycles, (addr - 0x2000) & 0x8);                              // PPU registers (repeat every 8 bytes)
        case 0x4000 ... 0x4017: break;                                                                                  // NES APU and I/O registers
        case 0x4018 ... 0x401F: return 0x00;                                                                            // Disabled
        case 0x4020 ... 0x5FFF: DEBUG("[READ] accessing expansion ROM.\n"); return 0x00;                                // Expansion rom
        case 0x6000 ... 0x7FFF: return workRAM->read(cycles, addr - 0x6000);                                            // Work RAM (normally in cartridge)
        case 0x8000 ... 0xFFFF: return cartridge->PRG->read(cycles, cartridge->mapper->getMappedPRG(addr - 0x8000));    // PRG ROM (mapped)
        default: break;
    }

    return 0x00;
}

void CPU_Bus::write(int64_t& cycles, uint16_t addr, uint8_t data) {

    switch(addr) {
        case 0 ... 0x1FFF: cpuRAM->write(cycles, addr & 0x07FF, data); return;                                                      // 2kB internal RAM
        case 0x2000 ... 0x3FFF: return PPU_registers->write(cycles, (addr - 0x2000) & 0x8, data); return;                           // PPU registers (repeat every 8 bytes)
        case 0x4000 ... 0x4017: return;                                                                                             // NES APU and I/O registers
        case 0x4018 ... 0x401F: return;                                                                                             // Disabled
        case 0x4020 ... 0x5FFF: DEBUG("[WRITE] accessing expansion ROM.\n"); return;                                                // Expansion rom
        case 0x6000 ... 0x7FFF: return workRAM->write(cycles, addr - 0x6000, data); return;                                         // Work RAM (normally in cartridge)
        case 0x8000 ... 0xFFFF: DEBUG("[WRITE] writing to cartridge!\n"); return; //cartridge->PRG->write(cycles, cartridge->mapper->getMappedPRG(addr - 0x8000), data); return; // PRG ROM (mapped)
        default: break;
    }

}

void CPU_Bus::reset() {
    delete cpuRAM, PPU_registers, workRAM;
}

CPU_Bus::~CPU_Bus() {
    reset();
}