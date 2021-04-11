#ifndef NES_CONSOLE_H
#define NES_CONSOLE_H


#include "Memory.h"
#include "CPU.h"

class Console {

public:
    Console();

    void loadROM(const char* filename);
    void run();

private:
    Memory mem;
    CPU cpu;

};


#endif //NES_CONSOLE_H
