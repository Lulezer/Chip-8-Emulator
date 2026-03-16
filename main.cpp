#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdint>
#include <SDL3/SDL.h>


/*
            00E0 - CLS (DONE)
            00EE - RET (DONE)
            0nnn - SYS addr
            1nnn - JP addr (DONE)
            2nnn - CALL addr (DONE)
            3xkk - SE Vx, byte (DONE)
            4xkk - SNE Vx, byte (DONE)
            5xy0 - SE Vx, Vy (DONE)
            6xkk - LD Vx, byte (DONE)
            7xkk - ADD Vx, byte (DONE)
            8xy0 - LD Vx, Vy (DONE)
            8xy1 - OR Vx, Vy (DONE)
            8xy2 - AND Vx, Vy (DONE)
            8xy3 - XOR Vx, Vy (DONE)
            8xy4 - ADD Vx, Vy (DONE)
            8xy5 - SUB Vx, Vy (DONE)
            8xy6 - SHR Vx {, Vy} (DONE)
            8xy7 - SUBN Vx, Vy (DONE)
            8xyE - SHL Vx {, Vy} (DONE)
            9xy0 - SNE Vx, Vy (DONE)
            Annn - LD I, addr (DONE)
            Bnnn - JP V0, addr (DONE)
            Cxkk - RND Vx, byte (DONE)
            Dxyn - DRW Vx, Vy, nibble
            Ex9E - SKP Vx
            ExA1 - SKNP Vx
            Fx07 - LD Vx, DT
            Fx0A - LD Vx, K
            Fx15 - LD DT, Vx
            Fx18 - LD ST, Vx
            Fx1E - ADD I, Vx
            Fx29 - LD F, Vx
            Fx33 - LD B, Vx
            Fx55 - LD [I], Vx
            Fx65 - LD Vx, [I]


*/



void drawDisplay(SDL_Renderer* renderer, bool display[])
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int scale = 10;

    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            if (display[y * 64 + x])
            {
                SDL_FRect pixel = {
                    (float)(x * scale),
                    (float)(y * scale),
                    (float)scale,
                    (float)scale
                };

                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    SDL_RenderPresent(renderer);
}


void HandleInput(bool& running, uint8_t key[16])
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            running = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            switch (event.key.key)
            {
            case SDLK_1: key[0x1] = 1; break;
            case SDLK_2: key[0x2] = 1; break;
            case SDLK_3: key[0x3] = 1; break;
            case SDLK_4: key[0xC] = 1; break;

            case SDLK_Q: key[0x4] = 1; break;
            case SDLK_W: key[0x5] = 1; break;
            case SDLK_E: key[0x6] = 1; break;
            case SDLK_R: key[0xD] = 1; break;

            case SDLK_A: key[0x7] = 1; break;
            case SDLK_S: key[0x8] = 1; break;
            case SDLK_D: key[0x9] = 1; break;
            case SDLK_F: key[0xE] = 1; break;

            case SDLK_Z: key[0xA] = 1; break;
            case SDLK_X: key[0x0] = 1; break;
            case SDLK_C: key[0xB] = 1; break;
            case SDLK_V: key[0xF] = 1; break;
            }
        }

        if (event.type == SDL_EVENT_KEY_UP)
        {
            switch (event.key.key)
            {
            case SDLK_1: key[0x1] = 0; break;
            case SDLK_2: key[0x2] = 0; break;
            case SDLK_3: key[0x3] = 0; break;
            case SDLK_4: key[0xC] = 0; break;

            case SDLK_Q: key[0x4] = 0; break;
            case SDLK_W: key[0x5] = 0; break;
            case SDLK_E: key[0x6] = 0; break;
            case SDLK_R: key[0xD] = 0; break;

            case SDLK_A: key[0x7] = 0; break;
            case SDLK_S: key[0x8] = 0; break;
            case SDLK_D: key[0x9] = 0; break;
            case SDLK_F: key[0xE] = 0; break;

            case SDLK_Z: key[0xA] = 0; break;
            case SDLK_X: key[0x0] = 0; break;
            case SDLK_C: key[0xB] = 0; break;
            case SDLK_V: key[0xF] = 0; break;
            }
        }
    }
}


uint8_t chip8_fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


bool loadROM(const char* filename, uint8_t memory[])
{
    FILE* file = fopen(filename, "rb");

    if (!file)
    {
        std::cout << "Failed to open ROM: " << filename << "\n";
        return false;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    if (size <= 0)
    {
        std::cout << "ROM size invalid: " << size << "\n";
        fclose(file);
        return false;
    }

    if (size > (4096 - 0x200))
    {
        std::cout << "ROM too large: " << size << "\n";
        fclose(file);
        return false;
    }

    size_t bytesRead = fread(&memory[0x200], 1, size, file);
    fclose(file);

    std::cout << "Loaded ROM: " << filename
        << " (" << size << " bytes)\n";
    std::cout << "Bytes actually read: " << bytesRead << "\n";

    std::cout << std::hex;
    std::cout << "First 16 ROM bytes in memory:\n";
    for (int i = 0; i < 16; i++)
    {
        std::cout << (int)memory[0x200 + i] << " ";
    }
    std::cout << "\n";

    return bytesRead == (size_t)size;
}



int main()
{
    constexpr uint16_t CLS = 0x00E0;
    constexpr uint16_t RET = 0x00EE;
    constexpr uint16_t HALT = 0x00FF; // your custom test opcode

    bool running = true;

    uint8_t memory[4096] = {

    };
    uint8_t V[16] = {};

    uint16_t pc = 0x200;
    uint16_t I = 0;

    uint16_t stack[16];
    uint8_t sp = 0;   // stack pointer

    uint8_t key[16] = {};

    bool display[64 * 32] = {};

	bool drawFlag = false;


    for (int i = 0; i < 80; i++)
    {
        memory[0x50 + i] = chip8_fontset[i];
    }


	uint8_t delayTimer = 0;

    // Keypad keymap
    bool keymap[16] = {
        SDLK_X,
        SDLK_1,
        SDLK_2,
        SDLK_3,
        SDLK_Q,
        SDLK_W,
        SDLK_E,
        SDLK_A,
        SDLK_S,
        SDLK_D,
        SDLK_Z,
        SDLK_C,
        SDLK_4,
        SDLK_R,
        SDLK_F,
        SDLK_V,
    };

    display[0] = true;
    display[1] = true;
    display[64] = true;
    display[65] = true;

    I = 0x300;




   // // ==========================
   // // Sprite data at 0x300
   // // ==========================
   // memory[0x300] = 0b11110000;
   // memory[0x301] = 0b10010000;
   // memory[0x302] = 0b10010000;
   // memory[0x303] = 0b11110000;
   //
   // // ==========================
   // // Program at 0x200
   // // ==========================
   //
   // // 0x200: Clear screen
   // memory[0x200] = 0x00; memory[0x201] = 0xE0;
   //
   // // 0x202: Wait for key, store in V0
   // memory[0x202] = 0xF0; memory[0x203] = 0x0A;
   //
   // // 0x204: Set V1 = 8 (Y position)
   // memory[0x204] = 0x61; memory[0x205] = 0x08;
   //
   // // 0x206: Set I = 0x300 (sprite address)
   // memory[0x206] = 0xA3; memory[0x207] = 0x00;
   //
   // // 0x208: Draw sprite at (V0, V1), height 4
   // memory[0x208] = 0xD0; memory[0x209] = 0x14;
   //
   // // 0x20A: Jump back to 0x202
   // memory[0x20A] = 0x12; memory[0x20B] = 0x02;

    memory[0x200] = 0x60; memory[0x201] = 0x0A; // V0 = A
    memory[0x202] = 0xF0; memory[0x203] = 0x29; // I = sprite for V0
    memory[0x204] = 0x61; memory[0x205] = 0x08; // V1 = 8
    memory[0x206] = 0x62; memory[0x207] = 0x08; // V2 = 8
    memory[0x208] = 0xD1; memory[0x209] = 0x25; // draw at (V1, V2), height 5
    memory[0x20A] = 0x12; memory[0x20B] = 0x0A; // loop forever



    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "CHIP-8",
        64 * 10,
        32 * 10,
        0
    );

    if (!window)
    {
        std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }


    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);



    if (!renderer)
    {
        std::cout << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    if (!loadROM(R"(C:\Users\Lulu\Downloads\tetris.rom)", memory))
    {
        return 1;
    }

    uint64_t lastTimerTick = SDL_GetTicks();

    while (running)
    {

        uint64_t currentTicks = SDL_GetTicks();

        if (currentTicks - lastTimerTick >= 16)
        {
            if (delayTimer > 0)
            {
                delayTimer--;
            }

        }


        HandleInput(running, key);
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
				running = false;
                return 0;
            }
        }

        
        
        uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];

        std::cout << "PC: 0x" << std::hex << pc
            << "  Opcode: 0x" << opcode << "\n";

        switch (opcode & 0xF000)
        {


        case 0x0000:
        {
            switch (opcode)
            {
            case 0x00E0:
            {
                for (int i = 0; i < 64 * 32; i++)
                    display[i] = false;

                drawFlag = true;
                break;
            }

            case 0x00EE:
            {
                sp--;
                pc = stack[sp];
                continue;
            }

            case 0x00FF:
                std::cout << "HALT\n";
                running = false;
                break;

            case 0x0000:
                std::cout << "NOP\n";
                break;

            default:
                std::cout << "Unknown 0x0000 opcode: 0x"
                    << std::hex << opcode << "\n";
                return 1;
            }
            break;
        }

        case 0x1000:
        {
            uint16_t addr = opcode & 0x0FFF;
            pc = addr;
            std::cout << "Jump to 0x" << std::hex << addr << "\n";
            continue;
        }

        case 0x2000:
        {
            uint16_t addr = opcode & 0x0FFF;

            stack[sp] = pc + 2;   // store return address
            sp++;

            pc = addr;

            std::cout << "Call 0x" << std::hex << addr << "\n";

            continue;
        }

        case 0x3000:
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t kk = opcode & 0x00FF;

            if (V[x] == kk)
                pc += 2;

            std::cout << "Skip if V[" << (int)x << "] == " << (int)kk << "\n";
            break;
        }

        case 0x4000:
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t kk = opcode & 0x00FF;

            if (V[x] != kk)
                pc += 2;

            std::cout << "Skip if V[" << (int)x << "] != " << (int)kk << "\n";
            break;
        }

        case 0x5000:
        {
            uint16_t x = (opcode & 0x0F00) >> 8;
            uint16_t y = (opcode & 0x00F0) >> 4;

            if (V[x] == V[y])
                pc += 2;
            std::cout << "Skip if V[" << (int)x << "] == V[" << (int)y << "]\n";
            break;
        }

        case 0x6000:
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t kk = opcode & 0x00FF;

            V[x] = kk;

            std::cout << "V[" << (int)x << "] = " << (int)kk << "\n";
            break;
        }

        case 0x7000:
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t kk = opcode & 0x00FF;

            V[x] += kk;

            std::cout << "V[" << (int)x << "] += " << (int)kk << "\n";
            break;
        }

        case 0x8000:
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;

            switch (opcode & 0x000F)
            {
            case 0x0:
                V[x] = V[y];
                std::cout << "V[" << (int)x << "] = V[" << (int)y << "]\n";
                break;

            case 0x1:
                V[x] = V[x] | V[y];
                break;

            case 0x2:
                V[x] = V[x] & V[y];
                break;

            case 0x3:
                V[x] = V[x] ^ V[y];
                break;

            case 0x4:
            {
                uint16_t sum = V[x] + V[y];

                if (sum > 255)
                    V[0xF] = 1;
                else
                    V[0xF] = 0;

                V[x] = sum & 0xFF;
                break;
            }

            case 0x5:
            {
                if (V[x] > V[y])
                    V[0xF] = 1;
                else
                    V[0xF] = 0;

                V[x] = V[x] - V[y];
                break;
            }

            case 0x6:
            {
                V[0xF] = V[x] & 0x1;
                V[x] >>= 1;
                break;
            }

            case 0x7:
                V[x] = V[y] - V[x];
                break;

            case 0xE:
            {
                if (V[x] >= 128)
                    V[0xF] = 1;
                else
                    V[0xF] = 0;

                V[x] *= 2;

                break;
            }


            default:
                std::cout << "Unknown 8XY opcode: 0x" << std::hex << opcode << "\n";
                return 1;
            }

            break;
        }

        case 0x9000:
        {
            uint16_t x = (opcode & 0x0F00) >> 8;
            uint16_t y = (opcode & 0x00F0) >> 4;

            if (V[x] != V[y])
                pc += 2;
            break;
        }

        case 0xA000:
        {
            I = opcode & 0x0FFF;
            std::cout << "Set I = 0x" << std::hex << I << "\n";
            break;
        }

        case 0xB000:
        {
            uint16_t addr = opcode & 0x0FFF;
            pc = V[0] + addr;
            continue;
        }

        case 0xC000:
        {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t kk = opcode & 0x00FF;

            V[x] = (rand() % 256) & kk;

            break;
        }

        case 0xD000:
        {

            uint8_t x = V[(opcode & 0x0F00) >> 8];
            uint8_t y = V[(opcode & 0x00F0) >> 4];
            uint8_t height = opcode & 0x000F;

            std::cout << "DRAW x=" << (int)x
                << " y=" << (int)y
                << " I=0x" << std::hex << I
                << " height=" << std::dec << (int)height << "\n";

            V[0xF] = 0;

            for (int row = 0; row < height; row++)
            {
                uint8_t spriteByte = memory[I + row];

                for (int col = 0; col < 8; col++)
                {
                    if ((spriteByte & (0x80 >> col)) != 0)
                    {
                        int px = (x + col) % 64;
                        int py = (y + row) % 32;
                        int index = py * 64 + px;


                        int onCount = 0;
                        for (int i = 0; i < 64 * 32; i++)
                        {
                            if (display[i])
                                onCount++;
                        }

                        std::cout << "Pixels on after draw: " << onCount << "\n";

                        if (display[index])
                        {
                            V[0xF] = 1;
                        }

                        display[index] ^= true;
                    }
                }
            }


            break;


        }

        case 0xF000:
        {
            uint8_t x = (opcode & 0x0F00) >> 8;

            switch (opcode & 0x00FF)
            {
            case 0x0A:
            {
                bool key_pressed = false;

                for (int i = 0; i < 16; i++)
                {
                    if (key[i])
                    {
                        V[x] = i;
                        key_pressed = true;
                        break;
                    }
                }

                if (!key_pressed)
                    continue;

                break;
            }

            case 0x29:
            {
                I = 0x50 + (V[x] * 5);
                break;
            }

            case 0x07:
            {
                V[x] = delayTimer;
                break;
            }

            case 0x15:
            {
                delayTimer = V[x];
                break;
            }


            case 0x1E:
            {
                I += V[x];
                break;
            }

            case 0x33:
            {
                uint8_t value = V[x];
                memory[I] = value / 100;
                memory[I + 1] = (value / 10) % 10;
                memory[I + 2] = value % 10;
                break;
            }

            case 0x55:
            {
                for (int i = 0; i <= x; i++)
                {
                    memory[I + i] = V[i];
                }
                break;

            }

            case 0x65:
            {
                for (int i = 0; i <= x; i++)
                {
                    V[i] = memory[I + i];
                }
                break;
            }
            default:
                std::cout << "Unknown F opcode: 0x" << std::hex << opcode << "\n";
                return 1;
            }

            break;
        }

        case 0xE000:
        {
            uint8_t x = (opcode & 0x0F00) >> 8;

            switch (opcode & 0x00FF)
            {
            case 0x9E: // Ex9E
            {
                if (key[V[x]])
                    pc += 2;
                break;
            }

            case 0xA1: // ExA1
            {
                if (!key[V[x]])
                    pc += 2;
                break;
            }

            default:
            {
                std::cout << "Unknown E opcode: 0x" << std::hex << opcode << "\n";
                return 1;
            }
            }

            break;
        }


        default:
        {
            std::cout << "Unknown opcode: 0x" << std::hex << opcode << "\n";
            return 1;
        }
        
        }

        drawDisplay(renderer, display);
        SDL_Delay(5);
        pc += 2;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
