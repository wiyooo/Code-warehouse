/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   DHT11温湿度传感器读取。
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./systick/bsp_SysTick.h"
#include "DHT11/bsp_dht11.h"
#include "./HC_SR04/HC_SR04.h"

#include "./beep/bsp_beep.h" 
#include "./systick/bsp_SysTick.h"
#include "./TouchPad/bsp_touchpad.h"

#define Delay_ms(x) Delay_us(1000*x)	 //单位ms

void DHT11_func(void);
void TIM2_IRQHandler(void);
void DHT11_TIM3_Configuration(void);
void HC_SR04_TIM3_Configuration(void);
void beep_open(void);

extern DHT11_Data_TypeDef DHT11_Data;

/* 记录触摸按键  */
sw_num sw_sign = no;

/* 超声波距离  */
float distance = 0;

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
  /*初始化USART，配置模式为 115200 8-N-1*/
  Debug_USART_Config();
 
	/* 初始化系统定时器 */
	SysTick_Init();
	
	/* DHT11初始化 */
	DHT11_GPIO_Config();
	
	/* 初始化电容按键 */
	TPAD_Init();
	
	/* 初始化蜂鸣器 */
	BEEP_GPIO_Config();
	
	
	
	/* 初始化超声波 */
	HC_SR04_init();
	
	printf("\r\n***野火F407开发板 dht11 温湿度传感器实验***\r\n");
	
  while(1)
	{
		if(TPAD_Scan(0))
		{
			if(sw_sign == no)
			{
				printf("\r\n***开启DHT11***\r\n");
				TIM_Cmd(TIM3, ENABLE);				
				sw_sign = DHT11;
				beep_open();			
				/* 初始化2s一检测温湿度 */
				DHT11_TIM3_Configuration();
			}	
			else if(sw_sign == DHT11)
			{
				sw_sign = HC_SR04;
				printf("\r\n***开启HC_SR04***\r\n");
				TIM_SetAutoreload(TIM3, (600-1));		
				TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				TIM_Cmd(TIM3, ENABLE);
				TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
				beep_open();					
			}			
			else if(sw_sign == HC_SR04)
			{
				TIM_Cmd(TIM3, DISABLE);
				printf("\r\n***关闭***\r\n");
				sw_sign = no;
				beep_open();
			}
		}	
		
//		if(sign) DHT11_func();
	} 

}






void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        // 执行功能函数
        // ...
			
				if(sw_sign == DHT11)
				{
					DHT11_func();
				}			
				else if(sw_sign == HC_SR04)
				{
					distance = HC_SR04_GetDistance() *2 / 58 ;
					printf("\r\n distance == %.2fcm\r\n",distance);
				}

        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}


void DHT11_TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 20000 - 1; // 定时2秒
    TIM_TimeBaseStructure.TIM_Prescaler = 8400 - 1; // 时钟频率为84MHz，分频系数为8400，计数频率为10kHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM3, ENABLE);
}


void beep_open(void)
{
	switch(sw_sign)
	{
		case DHT11: {/* bee一声 */
									BEEP_ON;
									Delay_ms(100);
									BEEP_OFF;
									break;      }
		
		case HC_SR04:{/* bee一声 */
									BEEP_ON;
									Delay_ms(100);
									BEEP_OFF;
									break;      }
		
		case no:{/* bee两声 */
										BEEP_ON;
										Delay_ms(100);
										BEEP_OFF;	
										Delay_ms(100);
										BEEP_ON;
										Delay_ms(100);
										BEEP_OFF;	
										break;
															}
		default: break;
	}	
}

/*********************************************END OF FILE**********************/

