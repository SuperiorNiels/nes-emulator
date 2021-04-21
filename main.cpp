#include <iostream>
#include "Console.h"
#include "SDL/Window_SDL.h"

const char* WINDOW_TITLE = "NES Emulator";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* args[])
{
    const char* bin_location = "../tests/data/functional_test_no_decimal_ops.bin";

    Console c;
    c.loadROM(bin_location);

    auto factory = WindowFactory_SDL();
    auto window = factory.createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
    window->init();
    window->attachConsole(&c);
    while(window->isOpen()) window->update();
    window->close();
    
    return 0;
}