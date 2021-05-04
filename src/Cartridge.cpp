#include "Cartridge.h"


Cartridge::Cartridge(const char* filename) {
    openROM(filename);
}

bool Cartridge::openROM(const char* filename) {
    /* Load iNES roms and create correct mappers */

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
        return false;
    }
    file.close();
    DEBUG("File loaded.\n");

    // Check iNES header
    if(!(buffer[0] == 0x4E && buffer[1] == 0x45 && buffer[2] == 0x53 && buffer[3] == 0x1A)) {
        DEBUG("Selected file is not in iNES format, failed to load.\n");
        return false;
    }
    DEBUG("Valid iNES file opened, configuring mapper.\n");

    // Check PRG and CHR rom sizes and copy data in memory
    // TODO: this is probably dangerous code, there has to be a better way
    nPRGBanks = buffer[4]; // Kb
    PRG = new Memory(nPRGBanks * 16384);
    memcpy(PRG->getMemoryPointer(), buffer + 16, PRG->getMemorySize());
    DEBUG("PRG size: %d * 16384 Kb\n", nPRGBanks);
    nCHRBanks = buffer[5]; // Kb
    CHR = new Memory(nCHRBanks * 8192);
    memcpy(CHR->getMemoryPointer(), buffer + 16 + PRG->getMemorySize(), CHR->getMemorySize());
    DEBUG("CHR size: %d * 8192 Kb\n", nCHRBanks);

    // Determine mapper
    uint8_t lower = (buffer[6] & 0b11110000) >> 4;
    uint8_t upper = buffer[7] & 0b11110000;
    uint8_t mapper_id = upper ^ lower;
    DEBUG("NES file loaded mapper ID: %d\n", mapper_id);

    switch(mapper_id) {
        case 0:
            mapper = new Mapper000(nPRGBanks, nCHRBanks);
            break;
        default:
            reset();
            DEBUG("NES ROM containes unsupported mapper (%d).\n", mapper_id);
            return false;
    }

    // Success
    DEBUG("ROM loaded successfully!\n");
    return true;
}

void Cartridge::reset() {
    delete mapper, PRG, CHR;
    nPRGBanks, nCHRBanks = 0, 0;
}

Cartridge::~Cartridge() {
    reset();
}