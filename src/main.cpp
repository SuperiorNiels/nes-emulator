#include <iostream>
#include "Console.h"
#include "SDL/Window_SDL.h"


const char* WINDOW_TITLE = "NES Emulator";
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int WinMain(int argc, char* args[])
{
	auto windowFactory = WindowFactory_SDL();
    auto window = windowFactory.createWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);

    while(window->isOpen()) {
        window->update();
    }

    window->close();
	//auto window = Window_SDL();

    Console c;
    c.loadROM("../6502_samples/program.hex");
    c.run();
    return 0;
}