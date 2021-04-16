#include <iostream>
#include "Console.h"


const char* WINDOW_TITLE = "NES Emulator";
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int main(int argc, char* args[])
{
    Console c;
    c.loadROM("../6502_samples/program.hex");
    c.run();
    
    return 0;
}