#include "key.h"
#include "oled.h"
#include "oled_print.h"
#include "uart_dma.h"
#include "soft_i2c.h"
#include "logo.h"

//选项的个数
#define options_NUM 4


//选项的图标
unsigned char * options[]=
{
	gImage_setting,
	gImage_stopwatch,
	gImage_TandH,
	gImage_flashlight
	 
};





void enter_select(u8 selected)
{
	switch (selected)
	{
	case 0:
		
		break;
		case 1:
		
		break;
			case 2:
		
		break;
	default:
		break;
	}
}

void menu_Refresh(u8 selected)
{
		u8 left;
	if (selected == 0)
	{
		left= options_NUM - 1;
	}else{
		left = selected - 1;
	}
	
  u8 right = ((selected+1)%options_NUM);
   

	OLED_ShowPicture(0,16,32,32,options[left],1);
	OLED_ShowPicture(48,16,32,32,options[selected],0);
	OLED_ShowPicture(96,16,32,32,options[right],1);
	OLED_Refresh();

}
// 传入一个当前所选择的菜单项，可由上一次调用返回
u8 menu(u8 cho)
{

	OLED_Clear();
	menu_Refresh(cho);
	u8 selected = cho;
	u8 key;
	while (1)
	{
		if (key = KEY_Get())
		{
			switch (key)
			{
			case KEY0_PRES:
				if (selected == 0)
                {
                    selected = options_NUM - 1;  // 0→最后一项
                }
                else
                {
                    selected--;
                }
menu_Refresh(selected);

				break;
				case KEY1_PRES:
					selected++;
					selected = selected%options_NUM;
				menu_Refresh(selected);
					break;
				case KEY2_PRES:
				OLED_Clear();
					return selected;
				break;
				case KEY3_PRES:
					enter_select(selected);//进入所选择的菜单项
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
	u8 key;
	// 主循环
	while (1)
	{
		// 读取RTC时间
		RTC_Date_Get();
		OLED_Printf_Line(0, "%02d/%02d/%02d",

										 g_RTC_Date.RTC_Year + 2000,
										 g_RTC_Date.RTC_Month,
										 g_RTC_Date.RTC_Date);
		OLED_Printf_Line(1, "%02d:%02d:%02d",
										 g_RTC_Time.RTC_Hours,
										 g_RTC_Time.RTC_Minutes,
										 g_RTC_Time.RTC_Seconds);
		OLED_Refresh_Dirty();
		delay_ms(1000);

		if (key = KEY_Get())
		{
			switch (key)
			{
			case KEY3_PRES:
         menu(0);
				break;

			default:
				break;
			}
		}
	}
}
