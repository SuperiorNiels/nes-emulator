#include <iostream>
#include "Console.h"


const char* WINDOW_TITLE = "NES Emulator";
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int main(int argc, char* args[])
{
    Console c;
    c.cpu.reset();
    c.loadROM("../6502_samples/6502_functional_test.bin");
    int32_t cycles = 100000;
    c.cpu.execute(cycles);
    
    return 0;
}