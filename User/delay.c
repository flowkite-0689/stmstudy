/**
 * @file delay.c
 * @brief STM32F4xx 延时函数模块实现
 * @author flowkite-0689
 * @version v2.0
 * @date 2025.11.10
 */

#include "delay.h"

/**
 * @brief 简单的循环延时函数
 * @param count 延时循环次数
 * @note 这是一个简单的软件延时，延时时间取决于CPU频率
 */
void Delay_Cycles(uint32_t count)
{
    while (count--) {
        __NOP();  // 空操作，防止编译器优化
    }
}

/**
 * @brief 毫秒级延时函数
 * @param ms 延时时间（单位：毫秒）
 * @note 基于168MHz系统时钟的简单延时，约1ms=32000个循环周期
 * @warning 此函数为软件延时，精度较低，不适合需要精确计时的场景
 */
void Delay_ms(uint32_t ms)
{
    uint32_t i;
    for (i = 0; i < ms; i++) {
        Delay_Cycles(32000);  // 168MHz 下大约1ms
    }
}

/**
 * @brief 微秒级延时函数
 * @param us 延时时间（单位：微秒）
 * @note 基于168MHz系统时钟的简单延时，约1us=32个循环周期
 * @warning 此函数为软件延时，精度较低，不适合需要精确计时的场景
 */
void Delay_us(uint32_t us)
{
    uint32_t i;
    for (i = 0; i < us; i++) {
        Delay_Cycles(32);  // 168MHz 下大约1us
    }
}
