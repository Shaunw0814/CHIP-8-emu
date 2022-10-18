#include "chip8.h"

#include <string.h>
#include <iostream>

void chip8::init(){
    //resetting values
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;
    
    clear(disp, sizeof(disp));
    clear(reg, sizeof(reg));
    clear(memory, sizeof(memory));
}

void chip8::clear(unsigned char* stuff, int size){
    memset(stuff, 0, (size_t)size);
}