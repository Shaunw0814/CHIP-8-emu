#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include <iostream>

#include "graphic.h"
#include "chip8.h"

int main(int argc, char **argv) 
{
    graphic window;
    chip8 chip;

    chip.init();

    for(int i = 0; i < sizeof(chip.disp); i++){
        if(chip.disp[i] != 0){
            std::cout << chip.disp[i] << std::endl;
        }
    }

    //setup graphics

    window.setup(chip);
    //emu.init();

    return 0;
}