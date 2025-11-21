#include "alarm_menu.h"

// 选项的图标
const unsigned char *alarm_menu_options[] =
    {
        gImage_add,      // 新建闹钟图标
        gImage_list      // 闹钟列表图标
};

void alarm_menu_Enter_select(u8 selected)
{
    switch (selected)
    {
    case 0:
        // 新建闹钟功能
        break;
    case 1:
        // 闹钟列表功能
        break;
    
    default:
        break;
    }
}

void alarm_menu_Ref(u8 selected)
{
    u8 right = ((selected + 1) % alarm_menu_options_NUM);

    OLED_ShowPicture(48, 16, 32, 32, alarm_menu_options[selected], 0);
    OLED_ShowPicture(96, 16, 32, 32, alarm_menu_options[right], 1);
    OLED_Refresh();
}

void alarm_menu()
{
    u8 flag_RE = 1;
    u8 key;
    u8 selected = 0;
 
    while (1)
    {
        if (flag_RE)
        {
            OLED_Clear();
            alarm_menu_Ref(selected);
            flag_RE = 0;
        }
        
        if ((key = KEY_Get()) != 0)
        {
            switch (key)
            {
            case KEY0_PRES:
                if (selected == 0)
                {
                    selected = alarm_menu_options_NUM - 1; // 0→最后一项
                }
                else
                {
                    selected--;
                }
                alarm_menu_Ref(selected);
                break;
                
            case KEY1_PRES:
                selected++;
                selected = selected % alarm_menu_options_NUM;
                alarm_menu_Ref(selected);
                break;
                
            case KEY2_PRES:
                OLED_Clear();
                return;
                
            case KEY3_PRES:
                flag_RE = 1;
                alarm_menu_Enter_select(selected); // 进入所选择的菜单项
                break;

            default:
                break;
            }
        }
    }
}
