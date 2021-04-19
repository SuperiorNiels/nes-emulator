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
    int32_t cycles = 0;
    c.cpu.execute(cycles);
    std::cout << "Ran for " << cycles << " cycles." << std::endl;
    
    return 0;
}