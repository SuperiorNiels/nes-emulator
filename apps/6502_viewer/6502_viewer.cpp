#include <iostream>

#include "Console.h"
#include "SDL/Window_SDL.h"
#include "views/MainGUI.h"

const char* WINDOW_TITLE = "6502 Emulator";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* args[])
{
    Console c;
    auto gui = MainGUI(&c);
    auto factory = WindowFactory_SDL();
    auto window = factory.createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
    window->init();
    window->addView(&gui);
    while(window->isOpen()) window->update();
    window->close();
    
    return 0;
}