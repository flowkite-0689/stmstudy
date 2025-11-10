#include "key.h"



/**
 * @brief  读取按键GPIO状态
 * @param  key_num: 按键编号(0-3)
 * @retval uint8_t: 0-按下, 1-释放
 * @note   使用位带操作直接读取按键状态
 */
uint8_t Read_GPIO_State(uint8_t key_num)
{
    switch (key_num)
    {
    case 0:
        return KEY0_STATE();
    case 1:
        return KEY1_STATE();
    case 2:
        return KEY2_STATE();
    case 3:
        return KEY3_STATE();
    default:
        return 1; // 默认为释放
    }
}

/**
 * @brief  按键消抖检测
 * @param  key_num: 按键编号(0-3)
 * @retval uint8_t: 0-确认按下, 1-未按下或抖动
 * @note   进行延时消抖确认
 */
uint8_t Key_Debounce(uint8_t key_num)
{
    // 延时消抖，确认按键按下
    Delay_ms(15);
    
    // 再次检测按键状态
    return Read_GPIO_State(key_num);
}

/**
 * @brief  等待按键释放
 * @param  key_num: 按键编号(0-3)
 * @retval None
 * @note   阻塞等待按键释放
 */
void Key_WaitForRelease(uint8_t key_num)
{
    while (Read_GPIO_State(key_num) == 0) // 等待按键释放
    {
        Delay_ms(10); // 小延时，降低CPU占用率
    }
}

/**
 * @brief  获取按键状态（消抖、确解、等待释放的完整按键检测实现）
 * @param  None
 * @retval uint8_t: 0~3分别对应KEY0~KEY3按下，0xFF表示无按键按下
 * @note   采用"快速检测+延时确认+等待释放"的处理逻辑
 */
uint8_t Key_Read(void)
{
    uint8_t key_pressed = 0xFF; // 初始化为无按键
    uint8_t current_state;

    // 1. 快速检测四个按键的输入引脚
    for (uint8_t i = 0; i < 4; i++)
    {
        current_state = Read_GPIO_State(i);

        // 2. 检测到按键按下（低电平）
        if (current_state == 0)
        {
            key_pressed = i; // 记录可能按下的按键
            break;           // 退出循环，不再检测其他按键
        }
    }

    // 3. 如果没有按键按下，直接返回0xFF
    if (key_pressed == 0xFF)
    {
        return 0xFF;
    }

    // 4. 消抖检测
    if (Key_Debounce(key_pressed) == 0)
    {
        // 5. 等待按键释放
        Key_WaitForRelease(key_pressed);
        
        // 6. 确认按键"按下-释放"过程后，返回按键编号
        return key_pressed;
    }

    // 如果是干扰抖动，说明没有有效按键
    return 0xFF;
}

