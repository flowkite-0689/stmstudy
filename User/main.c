#include "stm32f4xx.h"
#include "./myInit/myInit.h"
#include "key/key.h"
/**
 * @brief 控制LED状态
 * @param led_num LED编号(0-3)
 * @param state LED状态(0-点亮, 1-关闭)
 * @note   使用位带操作直接控制LED状态
 */
void LED_Control(uint8_t led_num, uint8_t state)
{
    switch (led_num)
    {
    case 0: // LED0
        LED0(state);
        break;
    case 1: // LED1
        LED1(state);
        break;
    case 2: // LED2
        LED2(state);
        break;
    case 3: // LED3
        LED3(state);
        break;
    }
}

/**
 * @brief 主函数
 */
int main(void)
{
    uint8_t key_num;
    static uint8_t led_state[4] = {0, 0, 0, 0}; // 每个LED的状态
    KEY_Initx(0);
    KEY_Initx(1);
    KEY_Initx(2);
    KEY_Initx(3);

    LED_Initx(0);
    LED_Initx(1);
    LED_Initx(2);
    LED_Initx(3);
		
	BEEP_Initx(0);

    // 初始化LED为熄灭状态（高电平）
    LED0(0);
    LED1(0);
    LED2(0);
    LED3(0);

    while (1)
    {
        // 检测按键
        key_num = Key_Read();

        // 如果有按键按下，切换对应LED的状态
        if (key_num < 4)
        {
            led_state[key_num] = !led_state[key_num];
            LED_Control(key_num, led_state[key_num]);
        }

        // 小延时，降低CPU占用率
        Delay_ms(10);
    }
}
