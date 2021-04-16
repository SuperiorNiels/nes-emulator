#ifndef NES_WINDOWSDL_H
#define NES_WINDOWSDL_H

#include <iostream>
#include <SDL2/SDL.h>

#include "Window.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl.h"
#include "backends/imgui_impl_opengl3.h"

class Window_SDL : public Window {
public:
    Window_SDL() : Window() {};
    Window_SDL(const char* title, const int width, const int height) : Window(title, width, height) {};
    
    bool init() override;
    void update() override;
    void close() override;

    ~Window_SDL() override {};

private:
    void updateEvents();
    void updateScreen();

    SDL_Window* window = nullptr;
    bool fps_toggle = false;
};


class WindowFactory_SDL : public WindowFactory {
public:
    Window_SDL* createWindow(const char* title, const int width, const int height) const { 
        return new Window_SDL(title, width, height);
    };
};

#endif //NES_WINDOWSDL_H