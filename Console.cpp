//
// Created by niels on 7/4/19.
//

#include "Console.h"

Console::Console() {
    mem = Memory();
    cpu = CPU();
}

void Console::loadROM(const char *filename) {
    mem.loadBinary(filename);
}