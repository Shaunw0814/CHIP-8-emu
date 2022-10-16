#include "chip8.h"

#include <string.h>

void chip8::init(){
    //resetting values
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;
    
    clear(disp);
    clear(reg);
    clear(memory);
}

void chip8::clear(unsigned char* stuff){
    memset(stuff, 0, (size_t)sizeof(stuff));
}