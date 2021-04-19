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

    while(window->isOpen()) window->update();
    window->close();
    
    return 0;
}