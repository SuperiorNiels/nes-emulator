#include "NESMemory.h"


NESMemory::NESMemory() {
    this->mem_size = 2048; // 2kb mem
    this->mem = new uint8_t[(int) this->mem_size]{};
    this->mem_initialized = true;
}

void NESMemory::loadROM(const char* filename) {
    /* Load iNES roms and create correct mappers */

    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return; } 
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
    if(PRG != nullptr) delete PRG;
    if(CHR != nullptr) delete CHR;
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
    
    DEBUG("NES ROM containes unsupported mapper (%d).\n", mapper);
    return;
}

uint8_t NESMemory::read(int64_t& cycles, uint16_t addr) {
    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return 0x00; } 
    cycles++; // alyways use 1 cycle for reading
    return mem[addr];
}

void NESMemory::write(int64_t& cycles, uint16_t addr, uint8_t data) {
    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return; } 
    cycles++;
    mem[addr] = data;
}

void NESMemory::reset() {
    if(!mem_initialized) { DEBUG("Memory not initialized yet (load_bin).\n"); return; } 
    memset(mem, 0, mem_size);
}

NESMemory::~NESMemory() {
    delete PRG, CHR, mapper;
}