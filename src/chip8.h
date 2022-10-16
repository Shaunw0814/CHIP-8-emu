#pragma once

class chip8{
public:

    //variables
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char reg[16];

    unsigned short pc;
    unsigned short I;

    unsigned char disp[64*32];

    unsigned char delay_timer;
    unsigned char sound_timer;

    unsigned short stack[16];
    unsigned short sp;

    unsigned char key[16];

    //functions
    void init();
    void clear(unsigned char* stuff);

private:

};
