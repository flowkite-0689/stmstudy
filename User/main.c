#include "led.h"
#include "key.h"
#include "beep.h"
// #include "debug.h"  // 移植到uart_dma.h中
#include "uart_dma.h"    // 使用DMA版本的调试函数
#include "dht11.h"
#include <stdio.h>
#include "timer_general.h"
#include "htim.h"
void bbbb()
{
	while (1)
	{
		if (KEY_Get())
		{
			return;
		}
		
		for (int i = 0; i <=50; i++)
		{
			Set_PWM_Percentage(i*2);
			delay_ms(10);	
		}
		for (int  i = 0; i <=50; i++)
		{
			Set_PWM_Percentage(100-i*2	);
			delay_ms(10);
		}
		
		
	}
	
}

void lllll()
{
	while (1)
	{
		if (KEY_Get())
		{
			return;
		}
		
		for (int i = 0; i <=50; i++)
		{
			Set_PWM_Percentage1(i*2);
			delay_ms(10);	
		}
		for (int  i = 0; i <=50; i++)
		{
			Set_PWM_Percentage1(100-i*2	);
			delay_ms(10);
		}
		
		
	}
}

/**
 * @brief 主函数
 */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断分组配置一次就行
	debug_init();
	uint8_t key;					 // 保存键值
	SysTick_Init();	 // 初始化延时函数
	LED_Init();			 // 初始化LED端
	KEY_Init();			 // 初始化与按键连接的硬件接口
	DHT11_Init();      // 初始化DHT11（在延时函数初始化后）
	LED_Set_All(0);	 // 点亮所有LED（低电平点亮）
	// 实际效果：LED0、LED1、LED2亮，LED3灭（红色LED3未点亮）
	TIM13_PWM_Init();
  TIM14_PWM_Init();
	// 发送初始化完成消息
	printf("System Ready - Send commands to control LEDs\r\n");
	Usart1_Send_String("Press keys to control remote LEDs\r\n");
	DHT11_Data_TypeDef DHT11_Data;

	while (1)
	{
		key = KEY_Get(); // 从中断获取键值
		
		// 处理环形缓冲区发送任务（非阻塞）
		uart_tx_task();
		
		// 处理串口命令
		Process_Usart_Command();
		
		if (key)
		{
			uint8_t result;
			switch (key)
			{
			case KEY0_PRES: // 读取温湿度数据
				
				printf("k0 pers\n");
				result = Read_DHT11(&DHT11_Data);
				if (result == 0) {
					printf("Temperature: %d.%dC, Humidity: %d.%d\r\n",  DHT11_Data.temp_int,  DHT11_Data.temp_deci , DHT11_Data.humi_int,DHT11_Data.humi_deci );
				} else {
					printf("Failed to read DHT11 sensor data\r\n");
				}
				break;
			case KEY1_PRES: // 控制LED1翻转
				// Usart1_Send_String("1c\r\n");
				printf("123456789qwertyuiopasdfgjj\n");
				break;
			case KEY2_PRES: // 控制LED2翻转
				
				printf("2c\r\n");
					bbbb();
			break;
			case KEY3_PRES: // 控制LED3翻转
				printf("3c\r\n");
				lllll();
				break;
			}
		}
		else
			delay_ms(10);
	}
}
