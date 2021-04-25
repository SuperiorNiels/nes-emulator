#ifndef NES_WINDOW_H
#define NES_WINDOW_H

#include <vector>

#include "Console.h"
#include "View.h"

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

    const int addView(View* view) {
        views.push_back(view);
        return views.size();
    }

    void clearViews() { views.clear(); } // does not delete the views 
    void removeView(const int view_index) {
        views.erase(views.begin() + view_index);
    }

    virtual ~Window() { delete title; };

protected:
    // Window vars
    const char* title = nullptr;
    int width, height;
    bool is_open;
    int fps = 60;

    const char* glsl_version;

    // Views to render
    std::vector<View*> views;
};


class WindowFactory {
public:
    virtual Window* createWindow(const char* title, const int width, const int height) const = 0;
};

#endif //NES_WINDOW_H
