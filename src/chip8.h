#pragma once

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

    unsigned char key[16];

    // functions
    void init();
    void read_rom(const char* rom_file);
    void emulate_cycle();

private:
    // variables


    // functions
    void clear(unsigned char* stuff, int size);
    void execute_opcode(unsigned short opcode);
    void draw_pixel(uint8_t x, uint8_t y);
};
