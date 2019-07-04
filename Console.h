//
// Created by niels on 7/4/19.
//

#ifndef NES_CONSOLE_H
#define NES_CONSOLE_H


#include "Memory.h"
#include "CPU.h"

class Console {

public:
    Console() = default;

    void loadROM(const char* filename);

private:
    Memory mem;
    CPU cpu;

};


#endif //NES_CONSOLE_H
