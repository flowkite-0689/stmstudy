#include "spi.h"

// 配置SPI参数
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	// 1) 时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	// 2) 配置引脚
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;        
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;    	// 复用
    GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed; 	// 高速
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;   	// 推挽
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;   
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;    		// 片选引脚    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;    	// 复用
    GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed; 	// 高速
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;   	// 推挽
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;   
	GPIO_Init(GPIOB, &GPIO_InitStruct);	
	SPI_NSS_H;
	
	// 3) 配置引脚复用功能
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1);
	
	// 4) 配置SPI参数 
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	// 双线全双工
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;		// 主模式
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;	// 数据宽度8位
	SPI_InitStruct.SPI_CPOL =SPI_CPOL_Low; 			// SCK空闲低电平.spi模式0
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;		// 第1边沿采集数据
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;			// 软件模式
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // 频率84M/2 = 42MHz
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;	// 高位先发
	SPI_InitStruct.SPI_CRCPolynomial = 0x7;			// 暂时没有用硬件CRC,该参数设置了也没用
	SPI_Init(SPI1, &SPI_InitStruct);
	
	// 5) 使能SPI
	SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI1_ReadWriteByte(uint8_t txData)
{
	// 等待发送缓冲区为空
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	
	// 发送数据
	SPI_I2S_SendData(SPI1, txData);
	
	// 等待接收缓冲区非空
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	
	return SPI_I2S_ReceiveData(SPI1);	// 读取并返回接收的数据
}

uint32_t W25Q128_ReadID(void)
{
	uint8_t manufacturer_id = 0;
    uint8_t memory_type_id = 0;
    uint8_t capacity_id = 0;
    uint32_t JedecDeviceID = 0;
	
	SPI_NSS_L;	// 片选
	SPI1_ReadWriteByte(W25X_JedecDeviceID);
	manufacturer_id = SPI1_ReadWriteByte(W25X_Dummy);
	memory_type_id = SPI1_ReadWriteByte(W25X_Dummy);
	capacity_id = SPI1_ReadWriteByte(W25X_Dummy);
	JedecDeviceID = manufacturer_id << 16 | memory_type_id << 8 | capacity_id;
	SPI_NSS_H;
	return JedecDeviceID;
}

// 等待不忙
void W25Q128_WaitForWriteEnd(void)
{
	uint8_t status = 0;
	SPI_NSS_L;	// 片选
	SPI1_ReadWriteByte(W25X_ReadStatusReg1);
	
	do
	{
		status = SPI1_ReadWriteByte(W25X_Dummy);
	}while (status & 1);
	SPI_NSS_H;
}

//
// 等待不忙
// 写

