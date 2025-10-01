#include <iostream>
#include <string>
#include <SDL.h>
#include <verilated.h>
#include "Vtop_module.h"
#include "svdpi.h"
using namespace std;
std::string module_name;
int width, height;
int Front_Porch_H, Sync_Pulse_H, Back_Porch_H;
int Front_Porch_V, Sync_Pulse_V, Back_Porch_V;
uint64_t start_ticks = SDL_GetPerformanceCounter();
uint64_t frame_count = 0;
typedef struct Pixel {
    uint8_t a, r, g, b;
} Pixel;

Pixel* framebuffer;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

bool prev_hsync = true, prev_vsync = true;
int x = 0, y = 0;
uint64_t end_ticks;
double duration ;
double fps ;
    
extern "C" void set_resolution(int w, int h) {
    width = w;
    height = h;
    std::cout << "Resolution set to " << width << "x" << height << std::endl;

    if (width == 640 && height == 480) {
        Front_Porch_H = 16; Sync_Pulse_H = 96; Back_Porch_H = 48;
        Front_Porch_V = 10; Sync_Pulse_V = 2; Back_Porch_V = 33;
    } else if (width == 800 && height == 600) {
        Front_Porch_H = 40; Sync_Pulse_H = 128; Back_Porch_H = 88;
        Front_Porch_V = 1; Sync_Pulse_V = 4; Back_Porch_V = 23;
    }
    else if (width == 800 && height == 600) {
        Front_Porch_H = 40; Sync_Pulse_H = 128; Back_Porch_H = 88;
        Front_Porch_V = 1; Sync_Pulse_V = 4; Back_Porch_V = 23;
        }
    else if (width == 1024 && height == 768) {
        Front_Porch_H = 24; Sync_Pulse_H = 136; Back_Porch_H = 160;
        Front_Porch_V = 3; Sync_Pulse_V = 6; Back_Porch_V = 29;
    }
    else if (width == 1280 && height == 960) {
        Front_Porch_H = 80; Sync_Pulse_H = 132; Back_Porch_H = 216;
        Front_Porch_V = 1; Sync_Pulse_V = 3; Back_Porch_V = 30;
    }
    
    else {
        std::cerr << "Unsupported resolution!" << std::endl;
        exit(1);
    }

    framebuffer = new Pixel[width * height];
    window = SDL_CreateWindow("VGA Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
   // cout<< "after window initilization  "<<endl;
    if (!window || !renderer || !texture) {
        std::cerr << "SDL resource creation failed!\n";
        return ;
    }
}

extern "C" void render_pixel(uint8_t r, uint8_t g, uint8_t b, int hsync, int vsync) {
   //cout<<"drawing the image "<<endl;
    if (!hsync || !prev_hsync) {
        x = (x == width) ? 0 : -Back_Porch_H;
        if (!vsync || !prev_vsync) {
            y = (y == height) ? 0 : -Back_Porch_V;
        } else if (y < height && prev_hsync) {
            y++;
        }
    } else if (x < width && prev_vsync) {
        x++;
    }
   //cout << x <<","<< y<<endl;
    prev_hsync = hsync;
    prev_vsync = vsync;
   //cout <<prev_hsync<<endl;
    if (x >= 0 && x < width && y >= 0 && y < height) {
        Pixel* p = &framebuffer[y * width + x];
        p->r = r;
        p->g = g;
        p->b = b;
        p->a = 0xFF;
        //cout << x <<","<< y<<","<<p->r<<endl;
        //cout<<p->r<< endl;
    }
    
}

int main(int argc, char** argv) {
    
    Verilated::commandArgs(argc, argv);
  
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Init Failed!\n";
        return 1;
    }
    Vtop_module* top = new Vtop_module;
    top->reset = 1;
   
    top->clk = 0; 
    top->clk = 1; 
    top->eval();
    top->reset = 0;
    top->eval();
    SDL_Event e;
    while (1) {
        top->clk = 1; top->eval();
        top->clk = 0; top->eval();

        if (x == 0 && y == height - 1) {
            SDL_UpdateTexture(texture, NULL, framebuffer, width * sizeof(Pixel));
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
	   
          SDL_PumpEvents();
          frame_count++;
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_Q]) {
        
        goto end;  
    }
        }
    }
    
end:
    end_ticks = SDL_GetPerformanceCounter();
    duration = ((double)(end_ticks-start_ticks))/SDL_GetPerformanceFrequency();
    fps = (double)frame_count/duration;
    //printf("Frames per second: %.1f\n", fps);
    cout<<"done drawing in c++"<<endl;
    delete top;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
