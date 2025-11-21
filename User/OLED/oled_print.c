#include "oled_print.h"

// 临时缓冲区用于格式化字符串
static char oled_buffer[128];

/**
 * @brief OLED打印函数 - 在指定位置格式化打印信息
 */
void OLED_Printf(uint8_t x, uint8_t y, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    
    // 格式化字符串
    vsnprintf(oled_buffer, sizeof(oled_buffer), format, args);
    
    // 显示字符串
    OLED_ShowString(x, y, (uint8_t*)oled_buffer, 12, 1);
    
    va_end(args);
}

/**
 * @brief OLED行打印函数 - 在指定行打印信息
 */
void OLED_Printf_Line(uint8_t line, const char* format, ...)
{
    if (line >= OLED_MAX_LINES) return; // 防止越界
    
    va_list args;
    va_start(args, format);
    
    // 计算Y坐标
    uint8_t y = line * OLED_LINE_HEIGHT;
    
    // 格式化字符串
    vsnprintf(oled_buffer, sizeof(oled_buffer), format, args);
    
    // 清除该行
    OLED_Clear_Line(line);
    
    // 显示字符串
    OLED_ShowString(0, y, (uint8_t*)oled_buffer, 12, 1);
    
    // 标记该行为脏区域，用于局部刷新
    OLED_Set_Dirty_Area(0, y, 127, y + OLED_LINE_HEIGHT - 1);
    
    va_end(args);
}

/**
 * @brief OLED行打印函数32px - 在指定行打印信息
 */
void OLED_Printf_Line_32(uint8_t line, const char* format, ...)
{
    if (line >= OLED_MAX_LINES) return; // 防止越界
    
    va_list args;
    va_start(args, format);
    
    // 计算Y坐标
    uint8_t y = line * OLED_LINE_HEIGHT;
    
    // 格式化字符串
    vsnprintf(oled_buffer, sizeof(oled_buffer), format, args);
    
    // 清除该行
    OLED_Clear_Line(line);
    
    // 显示字符串
    OLED_ShowString(0, y, (uint8_t*)oled_buffer, 24, 1);
    
    // 标记该行为脏区域，用于局部刷新
    OLED_Set_Dirty_Area(0, y, 127, y + (OLED_LINE_HEIGHT*2) - 1);
    
    va_end(args);
}


/**
 * @brief OLED清屏指定行
 */
void OLED_Clear_Line(uint8_t line)
{
    if (line >= OLED_MAX_LINES) return;
    
    uint8_t y = line * OLED_LINE_HEIGHT;
    
    // 绘制空格填充该行
    for (uint8_t x = 0; x < OLED_MAX_CHARS; x++) {
        OLED_ShowChar(x * 8, y, ' ', 12, 1);
    }
}

/**
 * @brief OLED显示系统信息
 */
void OLED_Display_Info(uint8_t mode, const char* info)
{
    // 清屏
    OLED_Clear();
    
    // 第一行显示模式
    OLED_Printf_Line(0, "Mode: %d", mode);
    
    // 第二行显示信息
    if (strlen(info) > OLED_MAX_CHARS) {
        // 信息太长，分两行显示
        char temp[OLED_MAX_CHARS + 1];
        strncpy(temp, info, OLED_MAX_CHARS);
        temp[OLED_MAX_CHARS] = '\0';
        OLED_Printf_Line(1, "%s", temp);
        
        if (strlen(info) > OLED_MAX_CHARS) {
            OLED_Printf_Line(2, "%s", info + OLED_MAX_CHARS);
        }
    } else {
        OLED_Printf_Line(1, "%s", info);
    }
}

/**
 * @brief OLED显示传感器数据
 */
void OLED_Display_Sensor(const char* sensor_name, float data1, float data2, const char* unit)
{
    // 清屏
    OLED_Clear();
    
    // 第一行显示传感器名称
    OLED_Printf_Line(0, "%s", sensor_name);
    
    // 第二行显示数据1
    OLED_Printf_Line(1, "Data1: %.2f %s", data1, unit);
    
    // 第三行显示数据2
    OLED_Printf_Line(2, "Data2: %.2f %s", data2, unit);
    
    // 第四行显示状态
    OLED_Printf_Line(3, "Status: Active");
}
