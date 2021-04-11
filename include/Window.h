#ifndef NES_WINDOW_H
#define NES_WINDOW_H


class Window {
public:
    Window() : title("Window"), width(500), height(500) {};
    Window(const char* title, const int width, const int height) : 
        title(title), width(width), height(height), is_open(true) {};
    
    virtual bool initWindow() = 0;
    virtual void update() = 0;
    virtual void closeWindow() = 0;

    bool isOpen() const { return is_open; };

    virtual ~Window() { delete title; };

protected:
    const char* title = nullptr;
    int width, height;
    bool is_open;
};


class WindowFactory {
public:
    virtual Window* createWindow(const char* title, const int width, const int height) const = 0;
};

#endif //NES_WINDOW_H
