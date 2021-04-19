#include <iostream>
#include "Console.h"


const char* WINDOW_TITLE = "NES Emulator";
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int main(int argc, char* args[])
{
    Console c;
    c.loadROM("D:\\4.Code\\NES-emu\\6502_samples\\test_roms\\functional_test_no_decimal_ops.bin");
    c.cpu.setCPUSignal(RESET, true);
    c.cpu.execute(100000000);
    printf("Program exit.\n");
    return 0;
}