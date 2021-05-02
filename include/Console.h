#ifndef NES_CONSOLE_H
#define NES_CONSOLE_H


#include "Memory.h"
#include "CPU.h"

class Console {

public:
    Console();

    void loadROM(const char* filename);
    void run();

    CPU cpu;
    // PPU ppu; TODO

    Memory cpuMem;
};


#endif //NES_CONSOLE_H
