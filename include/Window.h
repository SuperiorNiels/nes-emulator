#ifndef NES_WINDOW_H
#define NES_WINDOW_H

#include "Console.h"

class Window {
public:
    Window() : title("Window"), width(500), height(500), is_open(true) {};
    Window(const char* title, const int width, const int height) : 
        title(title), width(width), height(height), is_open(true) {};
    
    virtual bool init() = 0;
    virtual void update() = 0;
    virtual void close() = 0;

    bool isOpen() const { return is_open; };
    void setFPS(int fps) { this->fps = fps; };
    int getFPS() const { return fps; };

    void attachConsole(const Console* console) { this->console = console; }

    virtual ~Window() { delete title; };

protected:
    // Window vars
    const char* title = nullptr;
    int width, height;
    bool is_open;
    int fps = 60;

    // Console vars
    const Console* console = nullptr;
    const char* glsl_version;
};


class WindowFactory {
public:
    virtual Window* createWindow(const char* title, const int width, const int height) const = 0;
};

#endif //NES_WINDOW_H
