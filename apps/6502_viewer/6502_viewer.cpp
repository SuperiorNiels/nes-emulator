#include <iostream>

#include "CPU.h"
#include "RAM_Bus.h"
#include "SDL/Window_SDL.h"
#include "views/MainGUI.h"

const char* WINDOW_TITLE = "6502 Emulator";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* args[])
{
    auto cpu = CPU();
    auto bus = RAM_Bus(); // bus with only RAM
    cpu.attachBus(&bus);
    cpu.setCPUSignal(RESET, true);
    cpu.execute(1); // execute to reset cpu (set known state)
    cpu.setProgramCounter(0x0400); // test program starts at 0400

    auto gui = MainGUI(&cpu, &bus);
    auto factory = WindowFactory_SDL();
    auto window = factory.createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

    window->init();
    window->addView(&gui);
    while(window->isOpen()) window->update();
    window->close();

    return 0;
}