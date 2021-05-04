#ifndef NES_CARTRIDGE
#define NES_CARTRIDGE

#include <stdint-gcc.h>
#include <cstdlib>
#include <fstream>

#include "Mapper.h"
#include "Mapper000.h"
#include "Memory.h"


class Cartridge {
public:
    Cartridge() {};
    explicit Cartridge(const char* filename);

    void reset();
    bool openROM(const char* filename);

    ~Cartridge();

public: // everything public?
    uint32_t nPRGBanks = 0, nCHRBanks = 0;
    Mapper* mapper = nullptr;
    Memory* PRG = nullptr;
    Memory* CHR = nullptr;
};

#endif