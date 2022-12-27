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

#include <bits/stdc++.h>
#include "imfilebrowser.h"
#include "graphic.h"
#include "chip8.h"

void graphic::setup(chip8 chip){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return;
    }

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->width_px, this->height_px, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
    

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        
        if(show_display){
            makeDisplay(chip);
        }
        
        if(show_process){
            makeProcess(chip); 
        }

        if(show_config){
            makeConfig(file_dialog, chip);
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void graphic::makeDisplay(chip8 chip){
    // Graphics window calculation
    ImGui::SetNextWindowSize({(float)width_px /2, (float)height_px / 2});
    ImGui::SetNextWindowPos({0, 0});

    // Window - Graphics
    ImGui::Begin("Graphics", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
    {
        // Get the current window's draw list
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        
        for(int i = 0; i < (int)sizeof(chip.disp); i++){
            if(chip.disp[i] != 0){
                // Calculate the x and y coordinates of the pixel
                int x = i % 64;
                int y = i / 64;

                // Add a pixel
                draw_list->AddRectFilled(ImVec2(x * 10, y * 10), ImVec2(x * 10 + 10, y * 10 + 10), IM_COL32_WHITE);
            }
        }
    }
    ImGui::End();
}

void graphic::makeProcess(chip8 &chip){
    // Processor information window calculation
    ImGui::SetNextWindowSize({(float)width_px / 2, (float)height_px});
    ImGui::SetNextWindowPos({(float)width_px / 2, 0});

    // Window - Processor Information
    ImGui::Begin("Processor Information", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    {
        ImGui::Text("Program counter: %d", chip.pc);
        ImGui::Text("Index: %d", chip.I);
        ImGui::Text("V0: %d", chip.V[0]);
        ImGui::Text("V1: %d", chip.V[1]);
        ImGui::Text("V2: %d", chip.V[2]);
        ImGui::Text("V3: %d", chip.V[3]);
        ImGui::Text("V4: %d", chip.V[4]);
        ImGui::Text("V5: %d", chip.V[5]);
        ImGui::Text("V6: %d", chip.V[6]);
        ImGui::Text("V7: %d", chip.V[7]);
        ImGui::Text("V8: %d", chip.V[8]);
        ImGui::Text("V9: %d", chip.V[9]);
        ImGui::Text("VA: %d", chip.V[10]);
        ImGui::Text("VB: %d", chip.V[11]);
        ImGui::Text("VC: %d", chip.V[12]);
        ImGui::Text("VD: %d", chip.V[13]);
        ImGui::Text("VE: %d", chip.V[14]);
        ImGui::Text("VF: %d", chip.V[15]);


    }
    ImGui::End();
}

void graphic::makeConfig(ImGui::FileBrowser &file_dialog, chip8 &chip){
    // Config window calculation
    ImGui::SetNextWindowSize({(float)width_px / 2, (float)height_px / 2});
    ImGui::SetNextWindowPos({0, 320});

    // Window - Config
    ImGui::Begin("Config", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        
        // Button for ROM choosing
        if(ImGui::Button("Choose ROM")){
            file_dialog.Open();
        }
        
        ImGui::SameLine();

        // Button for beginning cycle
        if(rom_file != ""){
            if(ImGui::Button("Read Rom")){
                chip.read_rom(rom_file.c_str());
            }

            if(ImGui::Button("Emulate Cycle")){
                chip.emulate_cycle();
            }
        }
        
        const char* rom_print = rom_file == "" ? "(null)" : rom_file.c_str();
        ImGui::Text("Rom Path: %s", rom_print);
    }
    ImGui::End();

    // Choosing ROM file
    file_dialog.Display();

    if(file_dialog.HasSelected()){
        std::cout << "Selected filename " << file_dialog.GetSelected().string() << std::endl;
        rom_file = file_dialog.GetSelected().string();
        file_dialog.ClearSelected();
        chip.pc = 0x200;
    }
        
}