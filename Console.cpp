//
// Created by niels on 7/4/19.
//

#include "Console.h"

void Console::loadROM(const char *filename) {
    mem.loadBinary(filename);
}