#ifndef DEBUG_H
#define DEBUG_H

#include "stm32f4xx.h"
#include <string.h>

#define USART_RX_BUFFER_SIZE 64

uint32_t get_usart_rx_count(void);
uint8_t is_command_ready(void);
void debug_init(void);
void Usart1_Send_String(char *string);
uint8_t Usart1_Receive_String(char *buffer, uint16_t size);
void Process_Usart_Command(void);

#endif
