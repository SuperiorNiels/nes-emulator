#include "MainGUI.h"

MainGUI::MainGUI(CPU* cpu, NESMemory* mem) : cpu(cpu), mem(mem) {
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
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".nes,.rom,.bin", "../");
        loadRom = false;
    }  
    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            DEBUG("Seleceted file: %s\n", filePathName.c_str());
            mem->loadROM(filePathName.c_str());
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

        if(mem->PRG != nullptr) {
            ImGui::BeginChild("mem1", ImVec2(0, full_heigth / 3), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);
            ImGui::Text("NES CPU RAM (size: %d bytes)", mem->getMemorySize());
            ImGui::Separator();
            cpuRAM.DrawContents((void*) mem->getMemoryPointer(), mem->getMemorySize());
            ImGui::EndChild();

            ImGui::Separator();

            ImGui::BeginChild("mem2", ImVec2(0, full_heigth / 3), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);
            ImGui::Text("CARTRIDGE PRG ROM (size: %d bytes, split in %d banks.)", mem->PRG->getMemorySize(), mem->PRG_size);
            ImGui::Separator();
            PRG.DrawContents((void*) mem->PRG->getMemoryPointer(), mem->PRG->getMemorySize());
            ImGui::EndChild();
            
            ImGui::Separator();

            ImGui::BeginChild("mem3", ImVec2(0, full_heigth / 3), false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav);
            ImGui::Text("CARTRIDGE CHR ROM (size: %d bytes, split in %d banks.)", mem->CHR->getMemorySize(), mem->CHR_size);
            ImGui::Separator();
            CHR.DrawContents((void*) mem->CHR->getMemoryPointer(), mem->CHR->getMemorySize());
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