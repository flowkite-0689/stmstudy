#ifndef ADC_H   
#define ADC_H

#include "stm32f4xx.h"
void ADC1_Init(void);
float ADC_ConvertToVoltage(float vref);
void ADC3_Init(void);
float ADC3_ReadVoltage(float vref);
#endif
