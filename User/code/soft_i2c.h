#ifndef SOFT_I2C_H
#define SOFT_I2C_H

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"

#define SCL_H   PBout(8) = 1
#define SCL_L   PBout(8) = 0
#define SDA_H	PBout(9) = 1
#define SDA_L	PBout(9) = 0
#define SDAin	PBin(9)
#define I2C_DELAY  delay_us_no_irq(3)
void Soft_I2C_Init(void);
uint8_t Soft_I2C_Write_Byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
uint8_t Soft_I2C_Write_Bytes(uint8_t dev_addr, uint8_t reg_addr, uint32_t len, uint8_t *data);
uint8_t Soft_I2C_Read_Byte_From_Reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data);
uint8_t Soft_I2C_Read_Bytes_From_Reg(uint8_t dev_addr, uint8_t reg_addr, uint32_t len, uint8_t *data);
#endif

