#include <iostream>

#include "CPU.h"
#include "CPU_Bus.h"
#include "SDL/Window_SDL.h"
#include "views/MainGUI.h"

const char *WINDOW_TITLE = "NES Emulator";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char *args[])
{
    auto cpu = CPU();
    auto cartridge = Cartridge();
    auto cpu_bus = CPU_Bus(&cartridge);
    cpu.attachBus(&cpu_bus);

    auto gui = MainGUI(&cpu, &cpu_bus);
    auto factory = WindowFactory_SDL();
    auto window = factory.createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

    window->init();
    window->addView(&gui);
    while (window->isOpen())
        window->update();
    window->close();

    return 0;
}