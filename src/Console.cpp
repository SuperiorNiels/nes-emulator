#include "Console.h"

Console::Console() {
    // Init memory
    mem = Memory(0x1FFF);
    mem.reset();

    // Init cpu
    cpu = CPU();
    cpu.setResetVector(0x0400);
    cpu.setCPUSignal(RESET, true);
    cpu.attachMemeory(&mem);
    cpu.execute(1); // execute to reset cpu (set known state)
}

void Console::loadROM(const char *filename) {
    mem.loadBinary(filename);
}
