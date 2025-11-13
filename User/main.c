#include "led.h"
#include "key.h"
#include "beep.h"
#include "debug.h"

/**
 * @brief 可中断的LED流水灯效果函数
 */
void ls_interruptible(void)
{
	uint32_t led_state[4] = {0, 0, 0, 0};
	uint32_t flag = 0;

	for (uint32_t i = 0; i < 4;)
	{
		// 检查是否有按键中断，如果有则退出
		if (KEY_Get() != 0)
		{
			return;
		}

		for (uint32_t j = 0; j < 4; j++)
		{
			led_state[j] = 1;
			if (j == i)
			{
				led_state[j] = 0;
			}
		}

		if (flag)
		{
			LED0 = led_state[0];
			LED1 = led_state[1];
			LED2 = led_state[2];
			LED3 = led_state[3];
		}
		else
		{
			LED3 = led_state[0];
			LED2 = led_state[1];
			LED1 = led_state[2];
			LED0 = led_state[3];
		}

		i++;
		if (i == 4)
		{
			flag = !flag;
		}

		i = i % 4;
		delay_ms(130);
	}
}

void ysysled()
{

	uint32_t last_LED0 = 0;
	uint32_t last_LED1 = 0;
	while (1)
	{

		if (KEY_Get() != 0)
		{
			return;
		}
		if ((get_systick() - last_LED0) >= 500)
		{
			LED0 = !LED0;
			last_LED0 = get_systick();
		}

		if ((get_systick() - last_LED1) >= 800)
		{
			LED1 = !LED1;
			last_LED1 = get_systick();
		}
	}
}

/**
 * @brief 主函数
 */
int main(void)
{
	
	debug_init();
	u8 key;					 // 保存键值
	SysTick_Init();	 // 初始化延时函数
	LED_Init();			 // 初始化LED端口
	BEEP_Init();		 // 初始化蜂鸣器端口
	KEY_Init();			 // 初始化与按键连接的硬件接口
	KEY_EXTI_Init(); // 初始化按键外部中断
	LED_Set_All(0);	 // 点亮所有LED（低电平点亮）
	delay_ms(100);
	LED0 = 1;				 // 熄灭LED0
	LED1 = 1;				 // 熄灭LED1
	LED2 = 1;				 // 熄灭LED2
	LED3 = 1;				 // 熄灭LED3
	// 实际效果：LED0、LED1、LED2亮，LED3灭（红色LED3未点亮）
	
	// 发送初始化完成消息
	printf("System Ready - Send commands to control LEDs\r\n");
	Usart1_Send_String("Press keys to control remote LEDs\r\n");
	
	while (1)
	{
		key = KEY_Get(); // 从中断获取键值
		
		// 处理串口命令
		Process_Usart_Command();
		
		if (key)
		{
			switch (key)
			{
			case KEY0_PRES: // 控制LED0翻转
				printf("0c\r\n");
				break;
			case KEY1_PRES: // 控制LED1翻转
				Usart1_Send_String("1c\r\n");
				break;
			case KEY2_PRES: // 控制LED2翻转
				Usart1_Send_String("2c\r\n");
				break;
			case KEY3_PRES: // 控制LED3翻转
				Usart1_Send_String("3c\r\n");
				break;
			}
		}
		else
			delay_ms(10);
	}

	// while (1)
	// {
	// 	key = KEY_Get(); // 从中断获取键值

	// 	// 处理串口命令
	// 	Process_Usart_Command();
		
	// 	// 检查是否有新的串口数据接收
	// 	if(get_usart_rx_count() > last_rx_count)
	// 	{
	// 		last_rx_count = get_usart_rx_count();
	// 		sprintf(test_msg, "RX:%d, CMD:%d", (int)last_rx_count, is_command_ready());
	// 		Usart1_Send_String(test_msg);
	// 		Usart1_Send_String("\r\n");
	// 	}
		
	// 	if (key)
	// 	{
	// 		Usart1_Send_String("KEY PRESSED\r\n"); // 调试信息
	// 		switch (key)
	// 		{
	// 		case KEY0_PRES: // 启动流水灯效果
	// 			Usart1_Send_String("start ls");
	// 			ls_interruptible();
	// 			Usart1_Send_String("end ls");
	// 			break;
	// 		case KEY1_PRES: // 启动ysysled

	// 			ysysled();
	// 			break;
	// 		case KEY2_PRES: // 控制LED1翻转
	// 			LED1 = !LED1;
	// 			break;
	// 		case KEY3_PRES: // 控制LED2翻转
	// 			LED2 = !LED2;
	// 			break;
	// 		}
	// 	}
	// 	else
	// 		delay_ms(10);
	// }
}
