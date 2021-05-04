#include "MainGUI.h"

MainGUI::MainGUI(CPU* cpu, CPU_Bus* bus) : cpu(cpu), bus(bus) {
    //mem_edit.ReadOnly = true;
    //mem_edit.OptShowDataPreview = true;
    //mem_edit.OptShowOptions = false;
}

bool MainGUI::render() {
    int64_t tmp;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGuiWindowFlags wflags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar;
    ImGui::Begin("NES Memory", (bool*) true, wflags);

    if(executeRom) cpu->execute(cpu_speed); // execute X instruction

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
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".nes,.rom,.bin", "../../../data/");
        loadRom = false;
    }  
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            DEBUG("Seleceted file: %s\n", filePathName.c_str());
            bus->openROM(filePathName.c_str());
            cpu->setCPUSignal(RESET, true);
            cpu->execute(1);
        }
        
        ImGuiFileDialog::Instance()->Close();
    }

    ImGuiStyle& style = ImGui::GetStyle();
    
    // Main content
    float footer_size = ImGui::GetTextLineHeightWithSpacing() + style.ItemSpacing.y;
    ImGui::BeginChild("Content", ImVec2(0, -footer_size), false, 0);

        float full_heigth = ImGui::GetWindowHeight() - footer_size;

        if(bus->cartridge->PRG != nullptr) {
            ImGui::BeginChild("mem1", ImVec2(0, full_heigth / 4), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);
            ImGui::Text("NES CPU RAM (size: %d bytes)", bus->cpuRAM->getMemorySize());
            ImGui::Separator();
            cpuRAM.DrawContents((void*)  bus->cpuRAM->getMemoryPointer(),  bus->cpuRAM->getMemorySize());
            ImGui::EndChild();

            ImGui::Separator();

            ImGui::BeginChild("mem2", ImVec2(0, full_heigth / 4), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);
            ImGui::Text("CARTRIDGE PRG ROM (size: %d bytes, split in %d banks.)", bus->cartridge->PRG->getMemorySize(), bus->cartridge->nPRGBanks);
            ImGui::Separator();
            PRG.DrawContents((void*) bus->cartridge->PRG->getMemoryPointer(), bus->cartridge->PRG->getMemorySize());
            ImGui::EndChild();
            
            ImGui::Separator();

            ImGui::BeginChild("mem3", ImVec2(0, full_heigth / 4), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);
            ImGui::Text("CARTRIDGE CHR ROM (size: %d bytes, split in %d banks.)", bus->cartridge->CHR->getMemorySize(), bus->cartridge->nCHRBanks);
            ImGui::Separator();
            CHR.DrawContents((void*) bus->cartridge->CHR->getMemoryPointer(), bus->cartridge->CHR->getMemorySize());
            ImGui::EndChild(); 

            ImGui::BeginChild("cpu", ImVec2(0, full_heigth / 4), true, 0);
                // 6502 CPU state
                auto cpu_state = cpu->getCPUState();
                auto cpu_flags = cpu->getCPUFlags();
                auto cpu_cycles = cpu->getCPUExecutedCycles();
                auto cpu_instruction = cpu->getCurrentInstruction();
                auto current_reset_vector = cpu->getResetVector();

                ImGui::Text("Curr. Instr.: %s (%2x)", cpu_instruction.name, bus->read(tmp, cpu_state.PC));
                ImGui::SameLine();
                ImGui::Text("| PC: %4X | AC: %2X | X: %2X | Y: %2X | SP: %4X", cpu_state.PC, cpu_state.AC, cpu_state.X, cpu_state.Y, cpu_state.SP);        
                ImGui::Text("Flags: ");
                for(uint8_t i = 0; i < 8; i++) {
                    if(cpu_flags[i]) { ImGui::SameLine(); ImGui::Text("%c", flags_chr[i]); }
                    else { ImGui::SameLine(); ImGui::Text("-"); }
                }
                ImGui::SameLine(); ImGui::Text("\t Cycles: %ld", cpu_cycles);

                if (ImGui::BeginTable("checkers", 6)) {
                    ImGui::TableNextColumn();
                        if(ImGui::Button("NMI")) { cpu->setCPUSignal(NMI, true); cpu->execute(1); }
                        ImGui::SameLine(); if(ImGui::Button("IRQ")) { cpu->setCPUSignal(IRQ, true); cpu->execute(1); }
                        ImGui::SameLine(); if(ImGui::Button("RESET")) { cpu->setCPUSignal(RESET, true); cpu->execute(1); }

                    ImGui::TableNextColumn();
                        char reset_vector_buf[4];
                        ImGui::Text("CPU reset vector: ");
                        ImGui::TableNextColumn();
                        sprintf(reset_vector_buf, "%X", current_reset_vector); 
                        if (ImGui::InputText("##addr", reset_vector_buf, sizeof(reset_vector_buf) + 1, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
                            uint16_t reset_vector;
                            if (sscanf(reset_vector_buf, "%4hX", &reset_vector) == 1) {
                                cpu->setResetVector(reset_vector);
                                //cpu->setCPUSignal(RESET, true);
                                //cpu->execute(1);
                            }
                        }

                    ImGui::EndTable();
                }

                if (ImGui::BeginTable("checkers", 6)) {
                    ImGui::TableNextColumn(); ImGui::Checkbox("CPU Execute", &executeRom);
                    ImGui::TableNextColumn(); ImGui::Text("Speed (cycles):");
                    ImGui::TableNextColumn(); ImGui::SliderInt("", &cpu_speed, 1, 100000);
                    ImGui::EndTable();
                }

                if(ImGui::BeginTable("bottons", 6)) {
                    ImGui::TableNextColumn(); 
                        if(ImGui::Button("Run Step")) {
                            cpu->execute(1);
                        }

                    ImGui::TableNextColumn(); 
                        char run_cycles[16];
                        ImGui::Text("Run Cycles: ");
                        ImGui::TableNextColumn(); 
                        memset(run_cycles, 0, sizeof(run_cycles));
                        if (ImGui::InputText("##cycles", run_cycles, sizeof(run_cycles), ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue)) {
                            int64_t _cycles = 0;
                            if (sscanf(run_cycles, "%ld", &_cycles) == 1) {
                                cpu->execute(_cycles);
                            }
                        }
                    
                    ImGui::EndTable();
                }

            ImGui::EndChild();
        } else {
            ImGui::BeginChild("text", ImVec2(0, full_heigth), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);
            ImGui::Text("Load NES ROM (in iNES format) using the load ROM menu.");
            ImGui::EndChild(); 
        }

    ImGui::EndChild();

    ImGui::Separator();

    ImGui::BeginChild("performance", ImVec2(0, 0));
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::EndChild();
    
    ImGui::End();

    return true;
}