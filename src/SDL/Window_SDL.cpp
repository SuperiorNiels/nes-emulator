#include "SDL/Window_SDL.h"

bool Window_SDL::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        DEBUG("Error: %s\n", SDL_GetError());
        return false;
    }

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
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
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return false;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Init memory viewer window
    //mem_edit.ReadOnly = true;
    //mem_edit.OptShowDataPreview = true;
    //mem_edit.OptShowOptions = false;
    return true;
}

void Window_SDL::close() {
	// Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window_SDL::update() {
    // Check events and render
    updateEvents();
    updateScreen();
    //SDL_Delay(1000 / fps);
}

void Window_SDL::updateEvents() {
    SDL_Event event;
    bool keydown = false;
    while(SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) is_open = false;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE 
            && event.window.windowID == SDL_GetWindowID(window)) is_open = false;
    }
}

void Window_SDL::updateScreen() {
    int64_t tmp; // tmp variable for reading mememory (without increasing cpu cycles)

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGuiWindowFlags wflags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar;
    ImGui::Begin("NES Memory", (bool*) true, wflags);

    if(executeRom) console->cpu.execute(cpu_speed); // execute X instruction

    // Menu Bar
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
            ImGui::MenuItem("Load ROM", NULL, &loadRom); 
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Load ROM file Dialog
    if(loadRom) {
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".bin,.rom,.nes", "../");
        loadRom = false;
    }  
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            DEBUG("Seleceted file: %s\n", filePathName.c_str());
            console->loadROM(filePathName.c_str());
            console->cpu.setCPUSignal(RESET, true);
            console->cpu.execute(1);
        }
        
        ImGuiFileDialog::Instance()->Close();
    }

    ImGuiStyle& style = ImGui::GetStyle();
    
    // Main content
    ImGui::BeginChild("Content", ImVec2(0, -(ImGui::GetTextLineHeightWithSpacing() + 2 + style.ItemSpacing.y)), false, 0);
        // 6502 CPU state
        auto cpu_state = console->cpu.getCPUState();
        auto cpu_flags = console->cpu.getCPUFlags();
        auto cpu_cycles = console->cpu.getCPUExecutedCycles();
        auto cpu_instruction = console->cpu.getCurrentInstruction();

        // Memory viewer
        float footer_height = style.ItemSpacing.y + 20 + ImGui::GetTextLineHeightWithSpacing() * 4;
        ImGui::BeginChild("mem_view", ImVec2(0, -footer_height), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);
        if(followPC) {
            mem_edit.GotoAddr = cpu_state.PC;
            mem_edit.DataEditingTakeFocus = false;
        }
        mem_edit.DrawContents((void*) console->mem.getMemoryStartPointer(), MAX_MEM + 1);
        ImGui::EndChild();
        
        ImGui::Separator();

        ImGui::Text("Curr. Instr.: %s (%2x)", cpu_instruction.name, console->mem.read(tmp, cpu_state.PC));
        ImGui::SameLine();
        ImGui::Text("| PC: %4X | AC: %2X | X: %2X | Y: %2X | SP: %4X", cpu_state.PC, cpu_state.AC, cpu_state.X, cpu_state.Y, cpu_state.SP);        
        ImGui::Text("Flags: ");
        for(uint8_t i = 0; i < 8; i++) {
            if(cpu_flags[i]) { ImGui::SameLine(); ImGui::Text("%c", flags_chr[i]); }
            else { ImGui::SameLine(); ImGui::Text("-"); }
        }
        ImGui::SameLine(); ImGui::Text("\t Cycles: %ld", cpu_cycles);
        if (ImGui::BeginTable("checkers", 6)) {
            ImGui::TableNextColumn(); ImGui::Checkbox("CPU Execute", &executeRom);
            ImGui::TableNextColumn(); ImGui::Text("Speed (cycles):");
            ImGui::TableNextColumn(); ImGui::SliderInt("", &cpu_speed, 1, 100000);
            ImGui::TableNextColumn(); ImGui::Checkbox("Follow PC", &followPC);
            ImGui::EndTable();
        }

        if(ImGui::BeginTable("bottons", 6)) {
            ImGui::TableNextColumn(); 
            if(ImGui::Button("Run Step")) {
                console->cpu.execute(1);
            }
            ImGui::TableNextColumn(); 
            char run_cycles[16];
            ImGui::Text("Run Cycles: ");
            ImGui::TableNextColumn(); 
            memset(run_cycles, 0, sizeof(run_cycles));
            if (ImGui::InputText("##cycles", run_cycles, sizeof(run_cycles), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
                int64_t _cycles = 0;
                if (sscanf(run_cycles, "%d", &_cycles) == 1) {
                    console->cpu.execute(_cycles);
                }
            }
            ImGui::TableNextColumn(); 
            char reset_vector_buf[4];
            ImGui::Text("CPU reset vector: ");
            ImGui::TableNextColumn(); 
            sprintf(reset_vector_buf, "%X", 0x400); 
            if (ImGui::InputText("##addr", reset_vector_buf, sizeof(reset_vector_buf) + 1, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
                uint16_t reset_vector;
                if (sscanf(reset_vector_buf, "%4X", &reset_vector) == 1) {
                    console->cpu.setResetVector(reset_vector);
                    console->cpu.setCPUSignal(RESET, true);
                    console->cpu.execute(1);
                }
            }
            ImGui::EndTable();
        }
    ImGui::EndChild();

    ImGui::BeginChild("performance", ImVec2(0, 0));
    ImGui::Separator();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::EndChild();
    
    ImGui::End();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0, 0, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}
