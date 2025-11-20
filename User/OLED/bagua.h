#ifndef __BAGUA_H
#define __BAGUA_H

// 八卦动画帧数量
#define BAGUA_FRAME_COUNT 8

// 八卦图案尺寸（128x64像素）
#define BAGUA_WIDTH 128
#define BAGUA_HEIGHT 64

// 外部变量声明
extern const unsigned char bagua_frames[BAGUA_FRAME_COUNT][1024];

// 动画函数声明
void Show_Bagua_Animation(uint16_t delay_ms, uint8_t cycles);
void Show_Bagua_Static(uint8_t frame_index);

#endif