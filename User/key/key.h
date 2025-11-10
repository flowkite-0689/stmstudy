#ifndef _KEY_H_
#define _KEY_H_


#include "../myInit/myInit.h"

uint8_t Read_GPIO_State(uint8_t key_num);
uint8_t Key_Debounce(uint8_t key_num);
void Key_WaitForRelease(uint8_t key_num);
uint8_t Key_Read(void);



#endif

