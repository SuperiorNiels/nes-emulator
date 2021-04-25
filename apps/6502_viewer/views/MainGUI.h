#ifndef CPUVIEWER
#define CPUVIEWER

#include "View.h"
#include "Console.h"

#include "imgui.h"
#include "imgui_memory_editor/imgui_memory_editor.h"
#include "ImGuiFileDialog.h"


class MainGUI : public View {
public:

    MainGUI() = delete;
    MainGUI(Console* console);

    bool render() override;

private:
    Console* console = nullptr;

    // View state varialbes
    MemoryEditor mem_edit;
    bool loadRom = false;
    bool executeRom = false;
    bool followPC = false;
    int cpu_speed = 1;
};

#endif