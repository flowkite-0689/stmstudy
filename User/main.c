#include "code/led.h"
#include "code/key.h"
#include "code/beep.h"
#include "code/uart_dma.h" // 使用DMA版本的调试函数
#include "code/dht11.h"
#include <stdio.h>
#include "code/timer_general.h"
#include "code/htim.h"
#include "code/rtc_date.h"
#include "code/adc.h"
/**
 * @brief 主函数
 */
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	debug_init();
	uint8_t key;		// 保存键值
	SysTick_Init(); // 初始化延时函数
	LED_Init();			// 初始化LED端
	KEY_Init();			// 初始化与按键连接的硬件接口
	DHT11_Init();		// 初始化DHT11（在延时函数初始化后）
	LED_Set_All(1); // 点亮所有LED（低电平点亮）
	// 实际效果：LED0、LED1、LED2亮，LED3灭（红色LED3未点亮）
	// TIM13_PWM_Init();
	RTC_Date_Init();
	TIM14_PWM_Init();
	// 初始化 ADC3（PF7）
	ADC3_Init();
	ADC1_Init();
	float revoltage;
	float voltage;

	int flag = 0;
	while (1)
	{

		if (flag)
		{
			voltage = ADC3_ReadVoltage(3.3f);
			int percent = voltage * 100 / 3.3f;
			if (percent > 80)
			{
				Set_PWM_Percentage1(0);
			}
			else
			{

				Set_PWM_Percentage1((100 - percent) * 10);
			}
		}

		key = KEY_Get();
		Process_Usart_Command();
		switch (key)
		{
		case KEY0_PRES:
			printf("LED1 change\r\n");
			LED1 = !LED1;
			break;
		case KEY1_PRES:

			//
			//
			printf("Voltage: %.2fV\r\n", voltage);
			if (voltage < 1)
			{
				printf("light V");
			}
			else if (voltage < 1.3f)
			{
				printf("light IV");
			}
			else if (voltage < 1.7f)
			{
				printf("light III");
			}
			else if (voltage < 2.0f)
			{
				printf("light II");
			}
			else if (voltage < 2.3f)
			{
				printf("light I");
			}
			else if (voltage < 2.7f)
			{
				printf("drak I");
			}
			else if (voltage < 3.0f)
			{
				printf("drak II");
			}
			else if (voltage < 3.3f)
			{
				printf("drak III");
			}
			printf("\n");

			revoltage = ADC_ConvertToVoltage(3.3f);
			printf("reVoltage: %.2fV\r\n", revoltage);
			break;
		case KEY2_PRES:
			RTC_Date_Get();
			break;
		case KEY3_PRES:

			if (flag)
			{
				printf("Light sensor lamp off\r\n");
			}
			else
			{
				printf("Light sensor lamp on\r\n");
			}

			flag = !flag;
			break;
		default:
			break;
		}

		// 延时（根据需要调整）
		delay_ms(15);
	}
}
