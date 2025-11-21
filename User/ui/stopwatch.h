#ifndef _STOPWATCH_H_
#define _STOPWATCH_H_

#include "oled.h"
#include "key.h"
#include "code/timer_general.h"
#include "oled_print.h"

// 秒表状态结构体
typedef struct {
    uint8_t running;              // 运行状态
    uint32_t start_time;          // 开始时间(ms)
    uint32_t pause_time;          // 暂停累计时间(ms)
    uint32_t elapsed_time;        // 总经过时间(ms)
} StopwatchState;

// 更新间隔定义(ms)
#define STOPWATCH_UPDATE_INTERVAL 100     // 秒表更新间隔

void stopwatch(void);
void Display_Stopwatch(StopwatchState* state);

#endif
