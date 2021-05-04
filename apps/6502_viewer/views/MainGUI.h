#ifndef CPUVIEWER
#define CPUVIEWER

#include "View.h"
#include "CPU.h"
#include "RAM_Bus.h"

#include "imgui.h"
#include "imgui_memory_editor/imgui_memory_editor.h"
#include "ImGuiFileDialog.h"


class MainGUI : public View {
public:

    MainGUI() = delete;
    MainGUI(CPU* cpu, RAM_Bus* bus);

    bool render() override;

private:
    CPU* cpu = nullptr; 
    RAM_Bus* bus = nullptr;

    // View state varialbes
    MemoryEditor mem_edit;
    bool loadRom = false;
    bool executeRom = false;
    bool followPC = false;
    int cpu_speed = 1;
};

#endif