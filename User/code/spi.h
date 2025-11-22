#ifndef  SPI_H
#define  SPI_H

#include "stm32f4xx.h"
#include "sys.h"

#define SPI_NSS_H 	PBout(14) = 1
#define SPI_NSS_L 	PBout(14) = 0
#define W25X_Dummy  0xFF


#define W25X_JEDECID 		0xEF4018	// Ð¾Æ¬ID
#define W25X_JedecDeviceID  0x9F		// ¶ÁIDµÄÖ¸Áî
#define W25X_ReadStatusReg1 0x05		// ¶Á×´Ì¬¼Ä´æÆ÷1




void SPI1_Init(void);
uint8_t SPI1_ReadWriteByte(uint8_t txData);
uint32_t W25Q128_ReadID(void);

#endif

