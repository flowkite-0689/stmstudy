// simple_test_image.c
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SCALE 4
#define WIDTH 128
#define HEIGHT 64

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
uint8_t oled_buffer[WIDTH * HEIGHT / 8];

void oled_init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("OLED Simulator", 
                             SDL_WINDOWPOS_CENTERED, 
                             SDL_WINDOWPOS_CENTERED,
                             WIDTH * SCALE, 
                             HEIGHT * SCALE, 
                             SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetScale(renderer, SCALE, SCALE);
}

void oled_draw_pixel(int x, int y, int color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        int byte_index = x + (y / 8) * WIDTH;
        int bit_mask = 1 << (y % 8);
        
        if (color) {
            oled_buffer[byte_index] |= bit_mask;
        } else {
            oled_buffer[byte_index] &= ~bit_mask;
        }
    }
}

void oled_update_display() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            int byte_index = x + (y / 8) * WIDTH;
            int bit_mask = 1 << (y % 8);
            
            if (oled_buffer[byte_index] & bit_mask) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
    
    SDL_RenderPresent(renderer);
}

void oled_cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// 创建一个简单的测试图像 - 矩形边框和对角线
void create_test_image() {
    // 画边框
    for (int x = 10; x < 118; x++) {
        oled_draw_pixel(x, 10, 1);  // 上边
        oled_draw_pixel(x, 54, 1);  // 下边
    }
    for (int y = 10; y < 55; y++) {
        oled_draw_pixel(10, y, 1);   // 左边
        oled_draw_pixel(117, y, 1);  // 右边
    }
    
    // 画对角线
    for (int i = 0; i < 40; i++) {
        oled_draw_pixel(20 + i, 15 + i, 1);   // 左上到右下
        oled_draw_pixel(100 - i, 15 + i, 1);  // 右上到左下
    }
    
    // 画一个圆形（近似）
    int cx = 64, cy = 32, r = 15;
    for (int angle = 0; angle < 360; angle++) {
        float rad = angle * 3.14159 / 180;
        int x = cx + (int)(r * cos(rad));
        int y = cy + (int)(r * sin(rad));
        oled_draw_pixel(x, y, 1);
    }
}

int main() {
    oled_init();
    
    memset(oled_buffer, 0, sizeof(oled_buffer));
    
    // 创建测试图像
    create_test_image();
    
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
        
        oled_update_display();
        SDL_Delay(16);
    }
    
    oled_cleanup();
    return 0;
}