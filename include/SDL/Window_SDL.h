#ifndef NES_WINDOWSDL_H
#define NES_WINDOWSDL_H

#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>   

#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "Window.h"

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

    ImGuiIO io;
    SDL_GLContext gl_context;
    SDL_Window* window = nullptr;
};


class WindowFactory_SDL : public WindowFactory {
public:
    Window_SDL* createWindow(const char* title, const int width, const int height) const { 
        return new Window_SDL(title, width, height);
    };
};

#endif //NES_WINDOWSDL_H
