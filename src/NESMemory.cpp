#include "NESMemory.h"


NESMemory::NESMemory() {
    this->mem_size = 2048; // 2kb mem
    this->mem = new uint8_t[(int) this->mem_size]{};
    this->mem_initialized = true;
}

void NESMemory::loadROM(const char* filename) {
    /* Load iNES roms and create correct mappers */

    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return; } 
    reset(); // first reset memory

    DEBUG("Loading binary program: %s\n", filename);
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    DEBUG("File size: %ld\n", size);
    char buffer[size];
    if(!file.read(buffer, size)) {
        DEBUG("Something went wrong while reading file.\n");
        file.close();
        return;
    }
    file.close();
    DEBUG("File loaded.\n");

    // Check iNES header
    if(!(buffer[0] == 0x4E && buffer[1] == 0x45 && buffer[2] == 0x53 && buffer[3] == 0x1A)) {
        DEBUG("Selected file is not in iNES format, failed to load.\n");
        return;
    }
    DEBUG("Valid iNES file opened, configuring mapper.\n");

    // Check PRG and CHR rom sizes and copy data in memory
    // TODO: this is probably dangerous code, there has to be a better way
    PRG_size = buffer[4]; // Kb
    PRG = new DirectMemory(PRG_size * 16384);
    memcpy(PRG->getMemoryPointer(), buffer + 16, PRG->getMemorySize());
    DEBUG("PRG size: %d * 16384 Kb\n", PRG_size);
    CHR_size = buffer[5]; // Kb
    CHR = new DirectMemory(CHR_size * 8192);
    memcpy(CHR->getMemoryPointer(), buffer + 16 + PRG->getMemorySize(), CHR->getMemorySize());
    DEBUG("CHR size: %d * 8192 Kb\n", CHR_size);

    // Determine mapper
    uint8_t lower = (buffer[6] & 0b11110000) >> 4;
    uint8_t upper = buffer[7] & 0b11110000;
    uint8_t mapper_id = upper ^ lower;
    DEBUG("NES file loaded mapper ID: %d\n", mapper_id);

    switch(mapper_id) {
        case 0:
            mapper = new Mapper000(PRG_size, CHR_size);
            return;
        default:
            break;
    }

    // Create NES memory map
    workRAM = new DirectMemory(0x2000);
    PPU_registers = new DirectMemory(8); // 8 bytes 
    
    DEBUG("NES ROM containes unsupported mapper (%d).\n", mapper);
    return;
}

uint8_t NESMemory::read(int64_t& cycles, uint16_t addr) {
    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return 0x00; } 

    cycles++; // alyways use 1 cycle for reading
    int64_t tmp; // for accessing
    switch(addr) {
        case 0 ... 0x1FFF: return mem[addr & 0x07FF];                                       // 2kB internal RAM
        case 0x2000 ... 0x3FFF: return PPU_registers->read(tmp, addr & 0x2008);             // PPU registers (repeat every 8 bytes)
        case 0x4000 ... 0x4017: break;                                                      // NES APU and I/O registers
        case 0x4018 ... 0x401F: return 0x00;                                                // Disabled
        case 0x4020 ... 0x5FFF: DEBUG("[READ] accessing expansion ROM.\n"); return 0x00;    // Expansion rom
        case 0x6000 ... 0x7FFF: return workRAM->read(tmp, addr);                            // Work RAM (normally in cartridge)
        case 0x8000 ... 0xFFFF: return PRG->read(tmp, mapper->getMappedPRG(addr));          // PRG ROM (mapped)
        default: break;
    }

    return 0x00;
}

void NESMemory::write(int64_t& cycles, uint16_t addr, uint8_t data) {
    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return; } 

    cycles++; // alyways use 1 cycle for writing
    int64_t tmp; // for accessing
    switch(addr) {
        case 0 ... 0x1FFF: mem[addr & 0x07FF] = data;                                           // 2kB internal RAM
        case 0x2000 ... 0x3FFF: return PPU_registers->write(tmp, addr & 0x2008, data);          // PPU registers (repeat every 8 bytes)
        case 0x4000 ... 0x4017: break;                                                          // NES APU and I/O registers
        case 0x4018 ... 0x401F: break;                                                          // Disabled
        case 0x4020 ... 0x5FFF: DEBUG("[READ] accessing expansion ROM.\n"); break;              // Expansion rom
        case 0x6000 ... 0x7FFF: return workRAM->write(tmp, addr, data);                         // Work RAM (normally in cartridge)
        case 0x8000 ... 0xFFFF: return PRG->write(tmp, mapper->getMappedPRG(addr), data);       // PRG ROM (mapped)
        default: break;
    }

}

void NESMemory::reset() {
    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return; } 
    memset(mem, 0, mem_size);
    delete mapper, PRG, CHR, PPU_registers, workRAM;
}

NESMemory::~NESMemory() {
    reset();
}