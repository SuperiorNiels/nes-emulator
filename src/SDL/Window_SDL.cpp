#include "SDL/Window_SDL.h"

bool Window_SDL::init() {
    // Init SDL and gl context
    glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

	// Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    /*// Init imgui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;*/

    return true;
}

void Window_SDL::close() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Window_SDL::update() {
    // Check events and render
    updateEvents();
    updateScreen();
    SDL_Delay(1000 / fps);
}

void Window_SDL::updateEvents() {
    SDL_Event event;
    bool keydown = false;
    while(SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            is_open = false;
            std::cout << "Quitting application!" << std::endl;
            break;
        
        default:
            break;
        }
    }
}

void Window_SDL::updateScreen() {
    
}
