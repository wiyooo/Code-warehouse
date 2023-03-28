#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f4xx.h"
#include <stdio.h>


/* 输出打印开关  */
#define DEBUG_ON 1
#if DEBUG_ON
#define DEBUG(...) printf(__VA_ARGS__)
#else
#define DEBUG(...)
#endif


//引脚定义
/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK                         RCC_APB2Periph_USART1

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_RX_PIN                      GPIO_Pin_10
#define DEBUG_USART_RX_AF                       GPIO_AF_USART1
#define DEBUG_USART_RX_SOURCE                   GPIO_PinSource10

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_TX_PIN                      GPIO_Pin_9
#define DEBUG_USART_TX_AF                       GPIO_AF_USART1
#define DEBUG_USART_TX_SOURCE                   GPIO_PinSource9
/************************************************************/


//串口波特率
#define DEBUG_USART_BAUDRATE                    115200



void Debug_USART_Config(void);
//int fputc(int ch, FILE *f);

#endif /* __USART1_H */
