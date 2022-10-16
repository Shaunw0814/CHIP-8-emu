#pragma once

#include "chip8.h"

class graphic {
public:
    int width_px = 1280;
    int height_px = 640;

    bool show_display = true;
    bool show_process = true;
    bool show_config = true;
    
    void setup(chip8 chip);
    void makeDisplay(chip8 chip);
    void makeProcess();
    void makeConfig();

private:

};