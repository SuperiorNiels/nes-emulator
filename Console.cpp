//
// Created by niels on 7/4/19.
//

#include "Console.h"

Console::Console() {
    cpu.attachMemeory(&mem);
}

void Console::loadROM(const char *filename) {
    mem.loadBinary(filename);
    //mem.dump();
}

void Console::run() {
    cpu.execute();
}