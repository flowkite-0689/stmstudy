/**
 * @file hardware_def.h
 * @brief STM32F4xx 硬件引脚定义和位带操作宏
 * @author flowkite-0689
 * @version v2.0
 * @date 2025.11.10
 * 
 * 本文件统一管理项目中所有硬件引脚的定义，以及位带操作相关宏。
 * 采用分层架构设计，将硬件定义与具体功能实现分离。
 */

#ifndef _HARDWARE_DEF_H_
#define _HARDWARE_DEF_H_

#include "stm32f4xx.h"

// ==================================
// 硬件引脚定义
// ==================================

/**
 * @defgroup LED_Definitions LED引脚定义
 * @{
 */
#define LED0_PIN    GPIO_Pin_9   ///< LED0引脚定义
#define LED0_PORT   GPIOF        ///< LED0端口定义
#define LED0_NUM    9            ///< LED0引脚号数(用于位带操作)

#define LED1_PIN    GPIO_Pin_10  ///< LED1引脚定义
#define LED1_PORT   GPIOF        ///< LED1端口定义
#define LED1_NUM    10           ///< LED1引脚号数(用于位带操作)

#define LED2_PIN    GPIO_Pin_13  ///< LED2引脚定义
#define LED2_PORT   GPIOE        ///< LED2端口定义
#define LED2_NUM    13           ///< LED2引脚号数(用于位带操作)

#define LED3_PIN    GPIO_Pin_14  ///< LED3引脚定义
#define LED3_PORT   GPIOE        ///< LED3端口定义
#define LED3_NUM    14           ///< LED3引脚号数(用于位带操作)
/** @} */

/**
 * @defgroup KEY_Definitions 按键引脚定义
 * @{
 */
#define KEY0_PIN    GPIO_Pin_0   ///< 按键0引脚定义
#define KEY0_PORT   GPIOA        ///< 按键0端口定义
#define KEY0_NUM    0            ///< 按键0引脚号数(用于位带操作)

#define KEY1_PIN    GPIO_Pin_2   ///< 按键1引脚定义
#define KEY1_PORT   GPIOE        ///< 按键1端口定义
#define KEY1_NUM    2            ///< 按键1引脚号数(用于位带操作)

#define KEY2_PIN    GPIO_Pin_3   ///< 按键2引脚定义
#define KEY2_PORT   GPIOE        ///< 按键2端口定义
#define KEY2_NUM    3            ///< 按键2引脚号数(用于位带操作)

#define KEY3_PIN    GPIO_Pin_4   ///< 按键3引脚定义
#define KEY3_PORT   GPIOE        ///< 按键3端口定义
#define KEY3_NUM    4            ///< 按键3引脚号数(用于位带操作)
/** @} */

/**
 * @defgroup BEEP_Definitions 蜂鸣器引脚定义
 * @{
 */
#define BEEP0_PIN   GPIO_Pin_8   ///< 蜂鸣器0引脚定义
#define BEEP0_PORT  GPIOF        ///< 蜂鸣器0端口定义
#define BEEP0_NUM   8            ///< 蜂鸣器0引脚号数(用于位带操作)
/** @} */

// ==================================
// 位带操作宏定义
// ==================================

/**
 * @defgroup Bitband_Operations 位带操作宏定义
 * @brief 提供STM32F4xx位带操作的宏定义
 * @note 位带操作可以实现对单个位的原子访问，提高代码效率和可读性
 * @{
 */

/**
 * @brief 位带别名地址计算宏
 * @param addr 原始寄存器地址
 * @param bit 位数
 * @note 位带别名地址计算公式：外设别名基地址 + (原始寄存器地址-外设基地址)*32 + 引脚号数*4
 */
#define BITBAND(addr, bit) (*(volatile unsigned long *)((0x42000000U + ((addr - 0x40000000U) << 5) + ((bit) << 2))))

/**
 * @brief GPIO输出数据寄存器位带访问宏
 * @param PORT GPIO端口
 * @param PIN  引脚号
 * @return 指定引脚输出状态的位带别名
 */
#define GPIO_OUT(PORT, PIN) BITBAND((uint32_t)&PORT->ODR, PIN)

/**
 * @brief GPIO输入数据寄存器位带访问宏
 * @param PORT GPIO端口
 * @param PIN  引脚号
 * @return 指定引脚输入状态的位带别名
 */
#define GPIO_IN(PORT, PIN) BITBAND((uint32_t)&PORT->IDR, PIN)

/**
 * @brief GPIO置位宏（设置引脚为高电平）
 * @param PORT GPIO端口
 * @param PIN  引脚号
 * @note 使用BSRRL寄存器进行原子置位操作
 */
#define GPIO_SET(PORT, PIN) (PORT->BSRRL = (1UL << PIN))

/**
 * @brief GPIO复位宏（设置引脚为低电平）
 * @param PORT GPIO端口
 * @param PIN  引脚号
 * @note 使用BSRRH寄存器进行原子复位操作
 */
#define GPIO_RST(PORT, PIN) (PORT->BSRRH = (1UL << PIN))

/** @} */

// ==================================
// LED位带操作宏
// ==================================

/**
 * @defgroup LED_BitBand_Operations LED位带操作宏定义
 * @brief 提供LED控制的位带操作宏定义，支持直接对LED引脚进行位操作
 * @note LED硬件设计为低电平点亮，高电平熄灭
 * @{
 */

/**
 * @brief LED0控制宏
 * @param state LED状态：0-亮，1-灭
 * @note 使用位带操作直接控制LED0
 */
#define LED0(state) (state ? GPIO_SET(LED0_PORT, LED0_NUM) : GPIO_RST(LED0_PORT, LED0_NUM))

/**
 * @brief LED1控制宏
 * @param state LED状态：0-亮，1-灭
 * @note 使用位带操作直接控制LED1
 */
#define LED1(state) (state ? GPIO_SET(LED1_PORT, LED1_NUM) : GPIO_RST(LED1_PORT, LED1_NUM))

/**
 * @brief LED2控制宏
 * @param state LED状态：0-亮，1-灭
 * @note 使用位带操作直接控制LED2
 */
#define LED2(state) (state ? GPIO_SET(LED2_PORT, LED2_NUM) : GPIO_RST(LED2_PORT, LED2_NUM))

/**
 * @brief LED3控制宏
 * @param state LED状态：0-亮，1-灭
 * @note 使用位带操作直接控制LED3
 */
#define LED3(state) (state ? GPIO_SET(LED3_PORT, LED3_NUM) : GPIO_RST(LED3_PORT, LED3_NUM))

/**
 * @brief LED状态读取宏
 * @return LED状态：0-亮，1-灭
 */
#define LED0_STATE() GPIO_IN(LED0_PORT, LED0_NUM)
#define LED1_STATE() GPIO_IN(LED1_PORT, LED1_NUM)
#define LED2_STATE() GPIO_IN(LED2_PORT, LED2_NUM)
#define LED3_STATE() GPIO_IN(LED3_PORT, LED3_NUM)

/** @} */

// ==================================
// 蜂鸣器位带操作宏
// ==================================

/**
 * @defgroup BEEP_BitBand_Operations 蜂鸣器位带操作宏定义
 * @brief 提供蜂鸣器控制的位带操作宏定义
 * @{
 */
#define BEEP0(state) (state ? GPIO_SET(BEEP0_PORT, BEEP0_NUM) : GPIO_RST(BEEP0_PORT, BEEP0_NUM))
#define BEEP0_STATE() GPIO_IN(BEEP0_PORT, BEEP0_NUM)
/** @} */

// ==================================
// 按键位带操作宏
// ==================================

/**
 * @defgroup KEY_BitBand_Operations 按键位带操作宏定义
 * @brief 提供按键读取的位带操作宏定义
 * @note 按键硬件设计为下拉模式，按下时为低电平(0)，释放时为高电平(1)
 * @{
 */
#define KEY0_STATE() GPIO_IN(KEY0_PORT, KEY0_NUM)
#define KEY1_STATE() GPIO_IN(KEY1_PORT, KEY1_NUM)
#define KEY2_STATE() GPIO_IN(KEY2_PORT, KEY2_NUM)
#define KEY3_STATE() GPIO_IN(KEY3_PORT, KEY3_NUM)
/** @} */

// ==================================
// LED直接赋值实现方案
// ==================================

/**
 * @defgroup LED_Direct_Assign_LED 直接赋值实现
 * @brief 实现LED0=0这样的直接赋值语法
 * @note 在C语言中，使用全局变量和位带操作实现此功能
 * @{
 */

/**
 * @brief LED控制结构体定义
 * @note 通过重载赋值操作实现LED控制
 */
typedef struct {
    volatile uint32_t *port;  // GPIO端口指针
    uint32_t pin;             // 引脚号
    uint32_t bit_addr;        // 位带地址
} LED_Controller;

/**
 * @brief LED控制器赋值函数
 * @param led LED控制器指针
 * @param value 赋值（0-亮，1-灭）
 * @note 实现类似 LED0 = 0 的效果
 */
static inline void LED_Assign(LED_Controller *led, uint8_t value) {
    if (value) {
        GPIO_Set(led->port, led->pin);
    } else {
        GPIO_Reset(led->port, led->pin);
    }
}

/**
 * @brief LED控制器赋值操作符宏
 * @param led_name LED名称
 * @param value 赋值
 * @note 实现类似 LED0 = 0 的语法
 */
#define LED_ASSIGN(led_name, value) LED_Assign(&led_name, value)

/**
 * @brief LED控制器定义
 * @note 使用方法：LED_ASSIGN(LED0, 0); // 点亮LED0
 */
extern LED_Controller LED0;
extern LED_Controller LED1; 
extern LED_Controller LED2;
extern LED_Controller LED3;

/** @} */

#endif /* _HARDWARE_DEF_H_ */
