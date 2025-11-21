#include "key.h"
#include "oled.h"
#include "oled_print.h"
#include "uart_dma.h"
#include "soft_i2c.h"
#include "logo.h"
#include "ui.h"
#include "rtc_date.h" // ????RTC????
#include "MPU6050.h"
#include "MPU6050/eMPL/inv_mpu_dmp_motion_driver.h"
#include "simple_pedometer.h"
#include <stdlib.h> // ????abs????????
// ????????
#define options_NUM 6

/**
 * @brief ???????????
 * @param weekday ???????1-7??1=???????
 * @return ?????????????
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

// ????????
const unsigned char *options[] =
		{
				gImage_stopwatch,
				gImage_setting,
				gImage_TandH,
				gImage_flashlight,
				gImage_bell,
				gImage_step

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

	case 5:
		step(); // ????????
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
// ?????????????????????????��??��???
u8 menu(u8 cho)
{
	u8 flag_RE = 1;
	u8 selected = cho;

	u8 key;
	while (1)
	{
		delay_ms(10);
		if (flag_RE)
		{
			OLED_Clear();
			menu_Refresh(selected);

			flag_RE = 0;
		}

		if ((key = KEY_Get()) != 0)
		{
			switch (key)
			{
			case KEY0_PRES:
				if (selected == 0)
				{
					selected = options_NUM - 1; // 0?????????
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
				selected = enter_select(selected); // ???????????????
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
	OLED_Refresh(); // ????

	MPU_Init();

	// ??MPU6050??ID
	u8 device_id;
	MPU_Read_Byte(MPU_ADDR, MPU_DEVICE_ID_REG, &device_id);
	printf("MPU6050 Device ID: 0x%02X (Expected: 0x68)\r\n", device_id);

	if (device_id != MPU_ADDR)
	{
		printf("MPU6050 Device ID Mismatch!\r\n");
		OLED_Printf_Line(1, "MPU ID Error!");
		OLED_Refresh();
		delay_ms(2000);
	}
	else
	{
		printf("MPU6050 Device ID OK\r\n");
	}

	// ????????
	simple_pedometer_init();

	// ????????????DMP????????????????

	u8 key;
	u8 cho = 0;
	unsigned long last_count = 0;

	// ???????????
	static unsigned long loop_counter = 0;
	OLED_Clear();
	// ?????
	while (1)
	{
		// ??RTC??
		RTC_Date_Get();
		OLED_Printf_Line(0, "%02d/%02d/%02d     %s",

										 g_RTC_Date.RTC_Year + 2000,
										 g_RTC_Date.RTC_Month,
										 g_RTC_Date.RTC_Date,
										 get_weekday_name(g_RTC_Date.RTC_WeekDay));
		// 显示时间 32像素

		OLED_Printf_Line_32(1, " %02d:%02d:%02d",
												g_RTC_Time.RTC_Hours,
												g_RTC_Time.RTC_Minutes,
												g_RTC_Time.RTC_Seconds);

		// ???????????
		short ax, ay, az;
		MPU_Get_Accelerometer(&ax, &ay, &az);

		// ???????
		loop_counter++;

		// ?????????????????????
		simple_pedometer_update(ax, ay, az);
		unsigned long count = g_step_count;

		// ??????
		if (count != last_count)
		{
			printf("!!! STEP DETECTED: %ld -> %ld !!!\r\n", last_count, count);
			last_count = count;
		}

		// ??????
		static short last_ax = 0, last_ay = 0, last_az = 0;
		long accel_diff = abs(ax - last_ax) + abs(ay - last_ay) + abs(az - last_az);
		last_ax = ax;
		last_ay = ay;
		last_az = az;

		if (accel_diff > 5000) // ????????
		{
			printf("Movement: diff=%ld\r\n", accel_diff);
		}

		// ????????
		if (loop_counter % 10 == 0)
		{
			printf("Step: %ld\r\n", count);
		}

		printf("Current step: %ld\r\n", count);

		OLED_Printf_Line(3, "step : %lu", count); // ????
																							// ????
		OLED_Refresh_Dirty();
		delay_ms(50); // ???????????


		if ()
		{
			/* code */
		}
		
		if ((key = KEY_Get()) != 0)
		{
			switch (key)
			{
			case KEY3_PRES:
				cho = menu(cho);
				break;

			default:
				break;
			}
		}
	}
}
