#pragma once

#include "imgui.h"

#include <bits/stdc++.h>


class chip8{
public:
    // variables
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];

    unsigned short pc = 0x200;
    unsigned short I;

    unsigned char disp[64*32];

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[16];
    unsigned short sp;

    const ImGuiKey key[16] = {
        ImGuiKey_X,       // Key code 0: "X"
        ImGuiKey_1,       // Key code 1: "1"
        ImGuiKey_2,       // Key code 2: "2"
        ImGuiKey_3,       // Key code 3: "3"
        ImGuiKey_Q,       // Key code 4: "Q"
        ImGuiKey_W,       // Key code 5: "W"
        ImGuiKey_E,       // Key code 6: "E"
        ImGuiKey_A,       // Key code 7: "A"
        ImGuiKey_S,       // Key code 8: "S"
        ImGuiKey_D,       // Key code 9: "D"
        ImGuiKey_Z,       // Key code A: "Z"
        ImGuiKey_C,       // Key code B: "C"
        ImGuiKey_4,       // Key code C: "4"
        ImGuiKey_R,       // Key code D: "R"
        ImGuiKey_F,       // Key code E: "F"
        ImGuiKey_V        // Key code F: "V"
    };

    // Keypad       Keyboard
    // +-+-+-+-+    +-+-+-+-+
    // |1|2|3|C|    |1|2|3|4|
    // +-+-+-+-+    +-+-+-+-+
    // |4|5|6|D|    |Q|W|E|R|
    // +-+-+-+-+ => +-+-+-+-+
    // |7|8|9|E|    |A|S|D|F|
    // +-+-+-+-+    +-+-+-+-+
    // |A|0|B|F|    |Z|X|C|V|
    // +-+-+-+-+    +-+-+-+-+

    bool kp = false;
    bool emulate = false;
    bool prompt = false;

    std::mutex emulate_mutex;
    std::condition_variable cv;

    int cycle = 0;

    // functions
    chip8() : emulate(true) {}

    void init();
    void read_rom(const char* rom_file);
    void emulate_cycle();

private:
    // variables
    

    // functions
    void clear(unsigned char* stuff, int size);
    void execute_opcode(unsigned short opcode);
    void draw_pixel(int reg_x, int reg_y, int height);
};
