#include "chip8.h"
#include "imgui.h"

#include <bits/stdc++.h>
#include <iostream>

void chip8::init(){
    //resetting values
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;
    
    clear(disp, sizeof(disp));
    clear(V, sizeof(V));
    clear(memory, sizeof(memory));
}

void chip8::clear(unsigned char* stuff, int size){
    memset(stuff, 0, (size_t)size);
}

void chip8::emulate_cycle(){
    if(pc < sizeof(memory)){
        opcode = (memory[pc] << 8) | memory[pc+1];
        execute_opcode(opcode);
    }
}

void chip8::read_rom(const char* rom_file){

    std::ifstream input(rom_file, std::ios::binary);
    input.read((char*)&memory[0x200], sizeof(memory) - 0x200);
    input.close();
    
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
                std::cout << "0. return from a subroutine\n";
            }
            else{
                std::cout << "0. Execute machine language subroutine at address " << (int)second_nibble << (int)third_nibble << (int)fourth_nibble << " [CAN IGNORE] \n";
            }
            
            break;
        
        case 0x1:
            std::cout << "1. jump to address " << (int)second_nibble << (int)third_nibble << (int)fourth_nibble << "\n";
            pc = opcode & 0x0FFF;
            break;
        
        case 0x2:
            std::cout << "2. execute subroutine at address " << (int)second_nibble << (int)third_nibble << (int)fourth_nibble << "\n";
            stack[++sp] = pc;
            pc = opcode & 0x0FFF;
            break;
        
        case 0x3:
            std::cout << "3. skip next instruction if V" << (int)second_nibble << " == " << (int)third_nibble << (int)fourth_nibble << "\n";
            if(V[second_nibble] == (opcode & 0x00FF)){
                pc += 2;
            }
            break;
        
        case 0x4:
            std::cout << "4. skip next instruction if V" << (int)second_nibble << " != " << (int)third_nibble << (int)fourth_nibble << "\n";
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
            std::cout << "6. store number " << (int)third_nibble << (int)fourth_nibble << " in V" << (int)second_nibble << "\n";
            V[second_nibble] = (opcode & 0x00FF);
            break;

        case 0x7:
            std::cout << "7. add number " << (int)third_nibble << (int)fourth_nibble << " in V" << (int)second_nibble << "\n";
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
            std::cout << "A. store memory address " << (int)second_nibble << (int)third_nibble << (int)fourth_nibble << " in register index \n";
            I = opcode & 0x0FFF;
            pc += 2;
            break;

        case 0xB:
            std::cout << "B. jump to address " << (int)second_nibble << (int)third_nibble << (int)fourth_nibble  << " + V0" << "\n";
            pc += (opcode & 0x0FFF) + V[0];
            break;

        case 0xC:
            std::cout << "C. set V" << (int)second_nibble << "to a random number with a mask of " << (int)third_nibble << (int)fourth_nibble << " + V0" << "\n";
            V[second_nibble] = (std::rand() & 256) & (opcode & 0x00FF);
            break;

        case 0xD:
            std::cout << "D. Draw a sprite at position " << (int)second_nibble << ", " << (int)third_nibble << " with " << (int)fourth_nibble << " bytes of sprite data starting at the address stored in I. Set Vf to 01 if any pixels are changed to unset, and 00 otherwise \n";
            draw_pixel((int)second_nibble, (int)third_nibble);
            break;

        case 0xE:
            switch(third_nibble){
                case 0x9:
                    if(ImGui::IsKeyDown(V[(int)second_nibble])){
                        pc += 2;
                    }
                    break;
                case 0xA:
                    if(!ImGui::IsKeyDown(V[(int)second_nibble])){
                        pc += 2;
                    }
                    break;
            }
            std::cout << "E. Skip ???\n";
            break;
        case 0xF:
            std::cout << "F. stuff\n";
            break;
        default:
            std::cout << "Unknown opcode" << std::endl;
            break;
    }

    pc += 2;
}

void chip8::draw_pixel(uint8_t x, uint8_t y){
    disp[(x * 64) + y] = 1;
}