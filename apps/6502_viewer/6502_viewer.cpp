#include <iostream>

#include "CPU.h"
#include "DirectMemory.h"
#include "SDL/Window_SDL.h"
#include "views/MainGUI.h"

const char* WINDOW_TITLE = "6502 Emulator";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* args[])
{
    auto cpu = CPU();
    cpu.setResetVector(0x0400);
    cpu.setCPUSignal(RESET, true);
    auto mem = DirectMemory(0xFFFF); // 16kb memory
    mem.reset();
    auto gui = MainGUI(&cpu, &mem);
    cpu.attachMemeory(&mem);
    cpu.execute(1); // execute to reset cpu (set known state)

    auto factory = WindowFactory_SDL();
    auto window = factory.createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

    window->init();
    window->addView(&gui);
    while(window->isOpen()) window->update();
    window->close();

    return 0;
}