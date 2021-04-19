#include <iostream>
#include "Console.h"
#include "SDL/Window_SDL.h"

const char* WINDOW_TITLE = "NES Emulator";
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int main(int argc, char* args[])
{
    auto factory = WindowFactory_SDL();
    auto window = factory.createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
    window->init();

    Console c;
    c.loadROM("D:\\4.Code\\NES-emu\\6502_samples\\test_roms\\functional_test_no_decimal_ops.bin");
    c.cpu.setResetVector(0x0400); // start of the program
    c.cpu.setCPUSignal(RESET, true);
    c.cpu.execute(100000000);
    printf("Program exit.\n");

    while(window->isOpen()) window->update();
    window->close();
    
    return 0;
}