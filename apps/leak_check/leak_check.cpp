#include <iostream>

#include "Memory.h"
#include "SDL/Window_SDL.h"

const char* WINDOW_TITLE = "Leak Check (Memory)";
const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 50;


int main(int argc, char* args[])
{
    for(int i = 0; i < 20; i++) {
        auto mem = Memory(1000000000); // 1 Gb mem
        mem.reset();
    }

    auto factory = WindowFactory_SDL();
    auto window = factory.createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

    window->init();
    while(window->isOpen()) window->update();
    window->close();


    return 0;
}