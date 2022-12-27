#pragma once

#include "chip8.h"
#include "imgui.h"
#include "imfilebrowser.h"

class graphic {
public:
    int width_px = 1280;
    int height_px = 640;

    bool show_display = true;
    bool show_process = true;
    bool show_config = true;

    bool emulate = false;

    ImGui::FileBrowser file_dialog;
    std::string rom_file;

    //graphic functions
    void setup(chip8 chip);
    void makeDisplay(chip8 chip);
    void makeProcess(chip8 &chip);
    void makeConfig(ImGui::FileBrowser &file_dialog, chip8 &chip);

private:

};