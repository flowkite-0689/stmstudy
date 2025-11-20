// basic_usage.c - 基本使用示例
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// OLED显示参数
#define WIDTH 128
#define HEIGHT 64
#define SCALE 4

// 模拟OLED显存
uint8_t oled_buffer[WIDTH * HEIGHT / 8];

// 基础函数声明
void oled_init();
void oled_clear();
void oled_set_pixel(int x, int y, int value);
void oled_update_display();
void oled_draw_text(int x, int y, const char* text);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int main() {
    printf("OLED基础示例\n");
    
    // 初始化OLED
    oled_init();
    
    // 清屏
    oled_clear();
    
    // 显示文本
    oled_draw_text(10, 20, "Hello OLED!");
    oled_draw_text(10, 40, "Example Program");
    
    // 更新显示
    oled_update_display();
    
    // 等待用户关闭
    printf("按窗口关闭按钮或ESC键退出...\n");
    int running = 1;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = 0;
            }
        }
        
        SDL_Delay(16); // ~60 FPS
    }
    
    // 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    printf("程序结束\n");
    return 0;
}

void oled_init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL初始化失败: %s\n", SDL_GetError());
        exit(1);
    }
    
    window = SDL_CreateWindow("OLED基础示例", 
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             WIDTH * SCALE, HEIGHT * SCALE,
                             SDL_WINDOW_SHOWN);
    if (!window) {
        printf("窗口创建失败: %s\n", SDL_GetError());
        exit(1);
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("渲染器创建失败: %s\n", SDL_GetError());
        exit(1);
    }
    
    SDL_RenderSetScale(renderer, SCALE, SCALE);
    
    memset(oled_buffer, 0, sizeof(oled_buffer));
    printf("OLED初始化完成\n");
}

void oled_clear() {
    memset(oled_buffer, 0, sizeof(oled_buffer));
}

void oled_set_pixel(int x, int y, int value) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    
    int byte_index = x + (y / 8) * WIDTH;
    int bit_mask = 1 << (y % 8);
    
    if (value) {
        oled_buffer[byte_index] |= bit_mask;
    } else {
        oled_buffer[byte_index] &= ~bit_mask;
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

// 简单的文本绘制函数（仅支持ASCII）
void oled_draw_text(int x, int y, const char* text) {
    // 简单的8x8字体（每个字符用8x8像素表示）
    const uint8_t font_8x8[][8] = {
        {0x00,0x7E,0x81,0xA5,0x81,0x99,0x81,0x7E}, // A
        {0x00,0x7E,0xFF,0xDB,0xFF,0xC3,0xE7,0xFF}, // B
        // 这里可以添加更多字符，为简化只显示H
        {0x00,0xC3,0xC3,0xC3,0xFF,0xC3,0xC3,0xC3}, // H
        {0x00,0x7E,0xE7,0xC3,0xC3,0xC3,0xE7,0x7E}, // O
        {0x00,0x7C,0xC6,0x06,0x0C,0x18,0x30,0x7E}, // P
        {0x00,0x7C,0xC6,0x06,0x06,0x46,0xE6,0x7C}, // Q
        {0x00,0x7C,0xC6,0xC6,0xFF,0xC6,0xC6,0xC6}, // R
        {0x00,0x7E,0xE7,0xC3,0xC3,0xC3,0xE7,0x7E}, // S
        {0x00,0x00,0x18,0x3C,0x3C,0x18,0x00,0x00}, // !
        {0x00,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E}, // 空格边框
    };
    
    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        if (c < ' ' || c > 'Z') continue; // 简化处理
        
        int font_index = 0;
        switch(c) {
            case 'A': font_index = 0; break;
            case 'H': font_index = 1; break;
            case 'O': font_index = 2; break;
            case 'P': font_index = 3; break;
            case 'Q': font_index = 4; break;
            case 'R': font_index = 5; break;
            case 'S': font_index = 6; break;
            case '!': font_index = 7; break;
            default: font_index = 8; break; // 默认显示空格
        }
        
        // 绘制字符
        for (int row = 0; row < 8; row++) {
            uint8_t font_row = font_8x8[font_index][row];
            for (int col = 0; col < 8; col++) {
                if (font_row & (0x80 >> col)) {
                    oled_set_pixel(x + i * 9 + col, y + row, 1);
                }
            }
        }
    }
}