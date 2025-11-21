#include "key.h"
#include "oled.h"
#include "oled_print.h"
#include "uart_dma.h"
#include "soft_i2c.h"
#include "logo.h"
#include "ui.h"
#include "rtc_date.h" // 包含RTC头文件
#include "MPU6050.h"
#include "MPU6050/eMPL/inv_mpu_dmp_motion_driver.h"

// 选项的个数
#define options_NUM 5

/**
 * @brief 获取星期名称
 * @param weekday 星期几（1-7，1=星期一）
 * @return 星期名称字符串
 */
static const char *get_weekday_name(u8 weekday)
{
	static const char *weekday_names[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};
	if (weekday >= 1 && weekday <= 7)
	{
		return weekday_names[weekday - 1];
	}
	return "---";
}

// 选项图标
const unsigned char *options[] =
		{
				gImage_stopwatch,
				gImage_setting,
				gImage_TandH,
				gImage_flashlight,
				gImage_bell

};

u8 enter_select(u8 selected)
{
	switch (selected)
	{
	case 0:
		stopwatch();
		break;
	case 1:
		setting();
		break;
	case 2:
		TandH();

		break;
	case 3:
		flashlight();
		break;
case 4:
alarm_menu();
break;
	default:
		break;
	}
	return selected;
}

void menu_Refresh(u8 selected)
{
	u8 left;
	if (selected == 0)
	{
		left = options_NUM - 1;
	}
	else
	{
		left = selected - 1;
	}

	u8 right = ((selected + 1) % options_NUM);

	OLED_ShowPicture(0, 16, 32, 32, options[left], 1);
	OLED_ShowPicture(48, 16, 32, 32, options[selected], 0);
	OLED_ShowPicture(96, 16, 32, 32, options[right], 1);
	OLED_Refresh();
}
// 处理当前选择的菜单项，返回上一次的选择
u8 menu(u8 cho)
{
	u8 flag_RE = 1;
	u8 selected = cho;

	u8 key;
	while (1)
	{

		if (flag_RE)
		{
			OLED_Clear();
			menu_Refresh(selected);

			flag_RE = 0;
		}

		if ((key = KEY_Get())!=0)
		{
			switch (key)
			{
			case KEY0_PRES:
				if (selected == 0)
				{
					selected = options_NUM - 1; // 0变成最后一个
				}
				else
				{
					selected--;
				}
				menu_Refresh(selected);

				break;
			case KEY1_PRES:
				selected++;
				selected = selected % options_NUM;
				menu_Refresh(selected);
				break;
			case KEY2_PRES:
				OLED_Clear();
				return selected;
	
			case KEY3_PRES:
				flag_RE = 1;
				selected= enter_select(selected); // 处理当前选择的菜单项
				break;

			default:
				break;
			}
		}
	}
}

int main()
{

	KEY_Init();
	OLED_Init();
	debug_init();

	OLED_ShowPicture(32, 0, 64, 64, gImage_bg, 1);
	OLED_Refresh(); // 刷新缓存，到屏幕上面显示出来了
	
	MPU_Init();
	
	// 检查MPU6050设备ID
	u8 device_id;
	MPU_Read_Byte(MPU_ADDR, MPU_DEVICE_ID_REG, &device_id);
	printf("MPU6050 Device ID: 0x%02X\r\n", device_id);
	
	while (mpu_dmp_init())
	{
		printf("MPU6050 ERROR \r\n");
		OLED_Printf_Line(1, "MPU6050 ERROR");
		OLED_Refresh();
		delay_ms(500);
	}
	
	// 启用计步器功能
	if(enable_pedometer_feature() != 0)
	{
		printf("Failed to enable pedometer feature\r\n");
		OLED_Printf_Line(1, "Pedometer Error!");
		OLED_Refresh();
		delay_ms(2000);
	}
	
	dmp_set_pedometer_step_count(0); // 置零0
	dmp_set_pedometer_walk_time(0);	 // 置时间0
	
	printf("Pedometer initialized successfully\r\n");

	u8 key;
	u8  cho=0;
	unsigned long count;
	unsigned long walk_time;
	unsigned long last_count = 0;
	
	OLED_Clear();
	// 主循环
	while (1)
	{
		// 读取RTC时间
		RTC_Date_Get();
		OLED_Printf_Line(0, "%02d/%02d/%02d %s",
										 g_RTC_Date.RTC_Year + 2000,
										 g_RTC_Date.RTC_Month,
										 g_RTC_Date.RTC_Date,
										 get_weekday_name(g_RTC_Date.RTC_WeekDay));
		OLED_Printf_Line(1, "%02d:%02d:%02d",
										 g_RTC_Time.RTC_Hours,
										 g_RTC_Time.RTC_Minutes,
										 g_RTC_Time.RTC_Seconds);
		
		// 处理FIFO数据以更新计步器 - 更频繁处理
		for(int i = 0; i < 5; i++) // 每次循环处理5次FIFO
		{
			safe_pedometer_update();
		}
		
		// 获取步数和时间
		dmp_get_pedometer_step_count(&count);
		dmp_get_pedometer_walk_time(&walk_time);
		
		// 检测步数变化
		if(count != last_count)
		{
			printf("Step count: %ld (change: %ld)\r\n", count, count - last_count);
			last_count = count;
		}
		
		// 更新OLED显示
		OLED_Printf_Line(2,"step : %d",count);
		OLED_Printf_Line(3,"time : %ds",walk_time);
		OLED_Refresh_Dirty();
		delay_ms(500); // 减少延迟，提高响应速度

		if ((key = KEY_Get())!=0)
		{
			switch (key)
			{
			case KEY3_PRES:
				// 如果按住KEY3超过2秒，则重置步数
				{
					u8 press_count = 0;
					while(KEY_Get() == KEY3_PRES && press_count < 20) // 2秒 (20*100ms)
					{
						delay_ms(100);
						press_count++;
					}
					
					if(press_count >= 20) // 长按2秒
					{
						dmp_set_pedometer_step_count(0); // 重置步数
						dmp_set_pedometer_walk_time(0);  // 重置时间
						OLED_Printf_Line(2, "step reset!");
						OLED_Printf_Line(3, "time reset!");
						OLED_Refresh();
						delay_ms(1000);
					}
					else
					{
						cho = menu(cho);
					}
				}
				break;

			default:
				break;
			}
		}
	}
}