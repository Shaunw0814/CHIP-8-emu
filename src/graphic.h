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

class graphic {
public:
    int width_px = 1280;
    int height_px = 640;

    bool showDisplay = true;
    bool showProcess = true;
    bool showConfig = true;
    
    bool dispFull = false;

    void setup();
    void makeDisplay(bool dispFull);
    void makeProcess();
    void makeConfig();

private:

};