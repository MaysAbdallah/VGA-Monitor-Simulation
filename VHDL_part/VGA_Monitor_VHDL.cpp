// vga_monitor.cpp
#include <iostream>
#include <SDL.h>
#include <cstring>
#include <vhpi_user.h>
using namespace std ;
typedef void (*vhpiStartupFuncT)(void);

void update_display();
void finalize_display();
int Front_Porch_H , Sync_Pulse_H , Back_Porch_H ;
int Front_Porch_V ,Sync_Pulse_V , Back_Porch_V ;
uint64_t start_ticks = SDL_GetPerformanceCounter();
uint64_t frame_counter=0 ;
uint64_t end_ticks;
double duration ;
double fps ;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;

void initialize_display();
extern "C" void vhpi_startup();
extern "C" void vhpi_set_resolution( int width,  int height);
extern "C" void vhpi_draw_pixel(int r, int g, int b, int hsync, int vsync);
vhpiStartupFuncT vhpi_startup_routines[] = {
    vhpi_startup,
    NULL
};


typedef struct Pixel {
    uint8_t r, g, b,a;
} Pixel;
Pixel* framebuffer;
int x = 0, y = 0;
int Width=1280 , Height=720;
bool prev_hsync = true, prev_vsync = true;

void sdl_event_loop();

extern "C" void vhpi_startup() {
    printf("VHPI startup routine initialized.\n");
    initialize_display();
}

extern "C" void vhpi_set_resolution( int width,  int height) {
    cout << "setting the resolution"<<width<<","<<height<<"\n";
    
    if (width == 640 && height == 480) {
        Front_Porch_H = 16; Sync_Pulse_H = 96; Back_Porch_H = 48;
        Front_Porch_V = 10; Sync_Pulse_V = 2; Back_Porch_V = 33;
    } else if (width == 800 && height == 600) {
        Front_Porch_H = 40; Sync_Pulse_H = 128; Back_Porch_H = 88;
        Front_Porch_V = 1; Sync_Pulse_V = 4; Back_Porch_V = 23;
    }
    else if (width == 1024 && height == 768) {
        Front_Porch_H = 24; Sync_Pulse_H = 136; Back_Porch_H = 160;
        Front_Porch_V = 3; Sync_Pulse_V = 6; Back_Porch_V = 29;
    }
    else if (width == 1280 && height == 960) {
        Front_Porch_H = 80; Sync_Pulse_H = 136; Back_Porch_H = 216;
        Front_Porch_V = 1; Sync_Pulse_V = 3; Back_Porch_V = 30;
    }
    Width=width;
    Height=height ;
    framebuffer = new Pixel[width * height];
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("VGA VHPI Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
}
void wait_for_quit() {
    SDL_Event e;
    bool open = true;
    while (open) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) open = false;
        }
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_Q]) open = false;
        SDL_Delay(10);
    }
    finalize_display();
    exit(0);
}



extern "C" void vhpi_draw_pixel(int r, int g, int b, int hsync, int vsync) {
    static int pixel_counter = 0;
    

    const int visible_width = Width;
    const int visible_height = Height;

    if (!hsync || !prev_hsync) {
        x = (x == Width) ? 0 : -Back_Porch_H;
        if (!vsync || !prev_vsync) {
            y = (y == Height) ? 0 : -Back_Porch_V;
        } else if (y < Height && prev_hsync) {
            y++;
        }
    } else if (x < Width && prev_vsync) {
        x++;
    }

    if (x >= 0 && x < visible_width && y >= 0 && y < visible_height) {
        Pixel* p = &framebuffer[y * Width + x];
        p->r = r;
        p->g = g;
        p->b = b;
        p->a = 0xFF;
        pixel_counter++;
    }

    if (x == 0 && y == Height - 1) {
        update_display();
        SDL_PumpEvents();
        frame_counter++;
      //  std::cout << " Frame " << frame_counter << " drawn\n";
        pixel_counter = 0;
    }

    
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_Q]) {
        finalize_display(); 
        exit(0); 
    }

    prev_hsync = hsync;
    prev_vsync = vsync;
}

void initialize_display() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL Initialization Failed: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow("VGA Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window Creation Failed: %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Renderer Creation Failed: %s\n", SDL_GetError());
        exit(1);
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, Width, Height);
    if (!texture) {
        fprintf(stderr, "Texture Creation Failed: %s\n", SDL_GetError());
        exit(1);
    }
}

void update_display() {
   // cout << "updating the display \n";
    SDL_UpdateTexture(texture, nullptr, framebuffer, Width * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    
}


void finalize_display() {
    end_ticks = SDL_GetPerformanceCounter();
    duration = ((double)(end_ticks-start_ticks))/SDL_GetPerformanceFrequency();
    fps = (double)frame_counter/duration;
    //printf("Frames per second: %.1f\n", fps);
     delete[] framebuffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

