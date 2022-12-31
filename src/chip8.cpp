#include "chip8.h"
#include "imgui.h"

#include <bits/stdc++.h>
#include <iostream>
#include <chrono>
#include <thread>

void chip8::init(){
    //resetting values
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;
    
    clear(disp, sizeof(disp));
    clear(V, sizeof(V));
    clear(memory, sizeof(memory));
    
    memset(stack, 0, sizeof(stack));
}

void chip8::clear(unsigned char* stuff, int size){
    memset(stuff, 0, (size_t)size);
}

void chip8::emulate_cycle(){
    while(pc < sizeof(memory)){
        //check for emulate variable
        std::unique_lock<std::mutex> lock(emulate_mutex);
        cv.wait(lock, [this](){ return emulate; });

        opcode = (memory[pc] << 8) | memory[pc+1];
        execute_opcode(opcode);
        cycle++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void chip8::read_rom(const char* rom_file){

    init();

    std::ifstream input(rom_file, std::ios::binary);
    input.read((char*)&memory[0x200], sizeof(memory) - 0x200);
    input.close();

    std::cout << rom_file << " read in and stored to memory\n";
    cycle = 0;

    std::thread emulate_thread(&chip8::emulate_cycle, this);
    emulate_thread.detach();
}

void chip8::execute_opcode(unsigned short opcode){

    // Decode the opcode using bit shifting and masking
    unsigned char first_nibble = (opcode & 0xF000) >> 12;
    unsigned char second_nibble = (opcode & 0x0F00) >> 8;
    unsigned char third_nibble = (opcode & 0x00F0) >> 4;
    unsigned char fourth_nibble = opcode & 0x000F;

    // Decode the opcode using a switch statement
    switch (first_nibble) {

        case 0x0:
            if(third_nibble == 0xE && fourth_nibble == 0x0){
                std::cout << "0. clear the screen\n";
                clear(disp, sizeof(disp));
            }
            else if(third_nibble == 0xE && fourth_nibble == 0xE){
                std::cout << "0. return from a subroutine " << sp << "\n";
                pc = stack[--sp];
            }
            else{
                std::cout << "0. Execute machine language subroutine at address " << (int)second_nibble << (int)third_nibble << (int)fourth_nibble << " [CAN IGNORE] \n";
            }
            
            break;
        
        case 0x1:
            std::cout << "1. jump to address " << (int)(opcode & 0x0FFF) << "\n";
            pc = opcode & 0x0FFF;
            pc -= 2;
            break;
        
        case 0x2:
            std::cout << "2. execute subroutine at address " << (int)(opcode & 0x0FFF) << "\n";
            stack[sp++] = pc;
            pc = opcode & 0x0FFF;
            break;
        
        case 0x3:
            std::cout << "3. skip next instruction if V" << (int)second_nibble << " == " << (int)(opcode & 0x00FF) << "\n";
            if(V[second_nibble] == (opcode & 0x00FF)){
                pc += 2;
            }
            break;
        
        case 0x4:
            std::cout << "4. skip next instruction if V" << (int)second_nibble << " != " << (int)(opcode & 0x00FF) << "\n";
            if(V[second_nibble] != (opcode & 0x00FF)){
                pc += 2;
            }
            break;
        
        case 0x5:
            std::cout << "5. skip if V" << (int)second_nibble << " == V" << (int)third_nibble << "\n";
            if(V[second_nibble == V[third_nibble]]){
                pc += 2;
            }
            break;

        case 0x6:
            std::cout << "6. store number " << (int)(opcode & 0x00FF) << " in V" << (int)second_nibble << "\n";
            V[second_nibble] = (opcode & 0x00FF);
            break;

        case 0x7:
            std::cout << "7. add number " << (int)(opcode & 0x00FF) << " in V" << (int)second_nibble << "\n";
            V[second_nibble] += (opcode & 0x00FF);
            break;

        case 0x8:
            switch (fourth_nibble){
                case 0x0:
                    std::cout << "8. store value of V" << (int)second_nibble << " in V" << (int)third_nibble << "\n";
                    V[second_nibble] = V[third_nibble];
                    break;

                case 0x1:
                    std::cout << "8. set V" << (int)second_nibble << " or V" << (int)third_nibble << "\n";
                    V[second_nibble] = V[second_nibble] | V[third_nibble];
                    break;

                case 0x2:
                    std::cout << "8. set V" << (int)second_nibble << " and V" << (int)third_nibble << "\n";
                    V[second_nibble] = V[second_nibble] & V[third_nibble];
                    break;

                case 0x3:
                    std::cout << "8. set V" << (int)second_nibble << " xor V" << (int)third_nibble << "\n";
                    V[second_nibble] = V[second_nibble] ^ V[third_nibble];
                    break;

                case 0x4:
                    std::cout << "8. add V" << (int)second_nibble << " and V" << (int)third_nibble << "\n";
                    if(V[second_nibble] + V[third_nibble] >= 256){
                        V[0xF] = 1;
                    }
                    else{
                        V[0xF] = 0;
                    }
                    V[second_nibble] = (V[second_nibble] + V[third_nibble])& 0xFF;
                    break;

                case 0x5:
                    std::cout << "8. subtract V" << (int)second_nibble << " and V" << (int)third_nibble << "\n";
                    if(V[second_nibble] > V[third_nibble]){
                        V[0xF] = 1;
                    }
                    else{
                        V[0xF] = 0;
                    }
                    V[second_nibble] = V[third_nibble] - V[second_nibble];
                    break;

                case 0x6:
                    std::cout << "8. shift right operation \n";
                    V[second_nibble] = V[second_nibble] >> 1;
                    V[0xF] = V[second_nibble] & 0x01;
                    break;
                
                case 0x7:
                    std::cout << "8. subtract Vx from Vy \n";
                    if(V[third_nibble] > V[second_nibble]){
                        V[0xF] = 1;
                    }
                    else{
                        V[0xF] = 0;
                    }
                    V[second_nibble] = V[second_nibble] - V[third_nibble];
                    break;

                case 0xE:
                    std::cout << "8. shift left operation \n";
                    V[second_nibble] = V[second_nibble] << 1;
                    V[0xF] = V[second_nibble] & 0&01;
                    break;

            }
            break;

        case 0x9:
            std::cout << "9. skip if V" << (int)second_nibble << " != V" << (int)third_nibble << "\n";
            if(V[second_nibble] != V[third_nibble]){
                pc += 2;
            }
            break;

        case 0xA:
            std::cout << "A. store memory address " << (int)(opcode & 0x0FFF) << " in register index \n";
            I = opcode & 0x0FFF;
            break;

        case 0xB:
            std::cout << "B. jump to address " << (int)(opcode & 0x0FFF)  << " + V0" << "\n";
            pc = (opcode & 0x0FFF) + V[0];
            break;

        case 0xC:
            std::cout << "C. set V" << (int)second_nibble << "to a random number with a mask of " << (int)(opcode & 0x00FF) << " + V0" << "\n";
            V[second_nibble] = (std::rand() & 256) & (opcode & 0x00FF);
            break;

        case 0xD:
            std::cout << "D. Draw a sprite at position " << (int)second_nibble << ", " << (int)third_nibble << " with " << (int)fourth_nibble << " bytes of sprite data starting at the address stored in I. Set Vf to 01 if any pixels are changed to unset, and 00 otherwise \n";
            draw_pixel((int)second_nibble, (int)third_nibble, (int)fourth_nibble);
            break;

        case 0xE:
            switch(third_nibble){
                case 0x9:
                    std::cout << "E. if iskeydown " << ImGui::GetKeyName(key[V[second_nibble]]) << "\n";
                    if(ImGui::IsKeyDown(key[V[second_nibble]])){
                        pc += 2;
                    }
                    break;
                case 0xA:
                    std::cout << "E. if !iskeydown " << ImGui::GetKeyName(key[V[second_nibble]]) << "\n";
                    if(!ImGui::IsKeyDown(key[V[second_nibble]])){
                        pc += 2;
                    }
                    break;
                default:
                    std::cout << "E. unaccounted opcode" << "\n";
                    break;
            }
            break;

        case 0xF:
            switch(opcode & 0x00FF){
                case 0x07:
                    std::cout << "F. V" << (int)second_nibble << " = delay timer\n";
                    V[second_nibble] = delay_timer;
                    break;
                case 0x0A:
                    std::cout << "F. wait for key press, store value of key in V" << (int)second_nibble << "\n";
                    kp = false;
                    while(!kp){
                        for(int i = 0; i < 16; i++){
                            if(ImGui::IsKeyPressed(ImGui::GetKeyIndex(key[i]))){
                                V[second_nibble] = i;
                                kp = true;
                            }
                        }
                    }
                    break;
                case 0x15:
                    std::cout << "F. set delay timer = V" << (int)second_nibble << "\n";
                    delay_timer = V[second_nibble];
                    break;
                case 0x18: 
                    std::cout << "F. set sound timer = V" << (int)second_nibble << "\n";
                    sound_timer = V[second_nibble];
                    break;
                case 0x1E:
                    std::cout << "F. I = I + V" << (int)second_nibble << "\n";
                    I += second_nibble;
                    break;
                case 0x29:
                    std::cout << "F. set I = location of sprite for digit V" << (int)second_nibble << "\n";
                    I = V[second_nibble];
                    break;
                case 0x33:
                    std::cout << "F. store Binary-coded decimal representation of V" << (int)second_nibble << " in mem locations, I , I+1, and I+2 \n";
                    memory[I] = V[second_nibble] / 100;
                    memory[I + 1] = V[second_nibble] / 10 % 10;
                    memory[I + 2] = V[second_nibble] % 10;
                    break;
                case 0x55:
                    std::cout << "F. store registers V0 through V" << (int)second_nibble << " in memory starting at location I \n";
                    for(int i = 0; i <= second_nibble; i++){
                        memory[I + i] = V[i];
                    }
                    break;
                case 0x65:
                    std::cout << "F. read registers V0 through V" << (int)second_nibble << " from memory location starting at location I\n";
                    for(int i = 0; i <= second_nibble; i++){
                        V[i] = memory[I + i];
                    }
                    break;
                default:
                    std::cout << "F. unaccounted for opcode\n";
                    break;
            }
            break;

        default:
            std::cout << "Unknown opcode" << std::endl;
            break;
    }

    pc += 2;
}

void chip8::draw_pixel(int reg_x, int reg_y, int height){
    auto x = V[reg_x];
    auto y = V[reg_y];
    V[0xF] = 0;

    for(int yline = 0; yline < height; yline++){
        auto pixel = memory[I + yline];

        for (int xline = 0; xline < 8; xline++)
        {
            if ((pixel & (0x80 >> xline)) != 0)
            {
                if (disp[(x + xline + ((y + yline) * 64))] == 1)
                {
                    V[0xF] = 1;
                }
                disp[x + xline + ((y + yline) * 64)] ^= 1;
            }
        }
    }
}