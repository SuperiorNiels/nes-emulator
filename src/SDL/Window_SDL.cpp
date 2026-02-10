#include "SDL/Window_SDL.h"

#include <cstdlib>

namespace
{
    float clampFloat(float value, float min_value, float max_value)
    {
        if (value < min_value)
        {
            return min_value;
        }
        if (value > max_value)
        {
            return max_value;
        }
        return value;
    }
}

bool Window_SDL::init()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        DEBUG("Error: %s\n", SDL_GetError());
        return false;
    }

    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
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
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Init glew
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return false;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    ImGui::StyleColorsDark();

    float scale = ui_scale;
    if (scale <= 0.0f)
    {
        scale = 1.0f;
    }

    const char *env_scale = std::getenv("NES_UI_SCALE");
    if (env_scale && env_scale[0] != '\0')
    {
        char *end = nullptr;
        float parsed = std::strtof(env_scale, &end);
        if (end != env_scale && parsed > 0.1f)
        {
            scale = parsed;
        }
    }
    else
    {
        int display_index = SDL_GetWindowDisplayIndex(window);
        if (display_index >= 0)
        {
            float ddpi = 0.0f;
            float hdpi = 0.0f;
            float vdpi = 0.0f;
            if (SDL_GetDisplayDPI(display_index, &ddpi, &hdpi, &vdpi) == 0 && ddpi > 1.0f)
            {
                scale = ddpi / 96.0f;
            }
        }
    }

    scale = clampFloat(scale, 0.75f, 3.0f);
    applyUiScale(scale);

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

void Window_SDL::setUiScale(float scale)
{
    ui_scale = scale;
    if (ImGui::GetCurrentContext() != nullptr)
    {
        applyUiScale(ui_scale);
    }
}

void Window_SDL::applyUiScale(float scale)
{
    if (scale <= 0.0f)
    {
        return;
    }

    ImGuiStyle &style = ImGui::GetStyle();
    float delta = scale / applied_ui_scale;
    style.ScaleAllSizes(delta);
    ImGuiIO &io = ImGui::GetIO();
    io.FontGlobalScale = scale;
    ui_scale = scale;
    applied_ui_scale = scale;
}

void Window_SDL::close()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window_SDL::update()
{
    updateEvents();
    updateScreen();
}

void Window_SDL::updateEvents()
{
    SDL_Event event;
    bool keydown = false;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            is_open = false;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            is_open = false;
    }
}

void Window_SDL::updateScreen()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    for (auto view : views)
    {
        if (!view->render())
            DEBUG("View rendering failed.");
    }

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}
