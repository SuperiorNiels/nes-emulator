#ifndef CPUVIEWER
#define CPUVIEWER

#include "View.h"
#include "CPU.h"
#include "CPU_Bus.h"

#include "imgui.h"
#include "imgui_memory_editor/imgui_memory_editor.h"
#include "ImGuiFileDialog.h"


class MainGUI : public View {
public:

    MainGUI() = delete;
    MainGUI(CPU* cpu, CPU_Bus* bus);

    bool render() override;

private:
    CPU* cpu = nullptr; 
    CPU_Bus* bus = nullptr;

    // View state varialbes
    MemoryEditor cpuRAM;
    MemoryEditor PRG;
    MemoryEditor CHR;
    bool loadRom = false;
    bool executeRom = false;
    bool followPC = false;
    int cpu_speed = 1;
};

#endif