#include "SDL/Window_SDL.h"

bool Window_SDL::init() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
	} else {
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
		if(window == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return 0;
		} else {
			screenSurface = SDL_GetWindowSurface(window);
            updateScreen();
		}
	}
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
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(window);
}
