#include "Console.h"

Console::Console() {
    // Init memory
    cpuMem = Memory(2048);
    cpuMem.reset();

    // Init cpu
    cpu = CPU();
    cpu.setResetVector(0x0400);
    cpu.setCPUSignal(RESET, true);
    cpu.attachMemeory(&cpuMem);
    cpu.execute(1); // execute to reset cpu (set known state)
}

void Console::loadROM(const char *filename) {
    mem.loadBinary(filename);
}
