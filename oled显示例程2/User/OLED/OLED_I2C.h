#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "stm32f4xx.h"

#define EEPROM_I2C                          I2C1
#define EEPROM_I2C_CLK                      RCC_APB1Periph_I2C1
#define EEPROM_I2C_CLK_INIT								RCC_APB1PeriphClockCmd

#define EEPROM_I2C_SCL_PIN                  GPIO_Pin_8                 
#define EEPROM_I2C_SCL_GPIO_PORT            GPIOB                       
#define EEPROM_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SCL_SOURCE               GPIO_PinSource8
#define EEPROM_I2C_SCL_AF                   GPIO_AF_I2C1

#define EEPROM_I2C_SDA_PIN                  GPIO_Pin_9                  
#define EEPROM_I2C_SDA_GPIO_PORT            GPIOB                       
#define EEPROM_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define EEPROM_I2C_SDA_SOURCE               GPIO_PinSource9
#define EEPROM_I2C_SDA_AF                   GPIO_AF_I2C1

#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);

#endif
