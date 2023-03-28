/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   串口接发测试，串口接收到数据后马上回传。
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
#include "./uart/bsp_debug_uart.h"
#include "./systick/bsp_SysTick.h"
#include "DHT11/bsp_dht11.h"
#include "./HC_SR04/HC_SR04.h"

#include "./beep/bsp_beep.h" 
#include "./systick/bsp_SysTick.h"
#include "./TouchPad/bsp_touchpad.h"
#include "./OLED/OLED_I2C.h"
#include "./dwt_delay/core_delay.h"

extern DHT11_Data_TypeDef DHT11_Data;

/* 记录触摸按键  */
sw_num sw_sign = no;

/* 超声波距离  */
float distance = 0 , dt = 0;

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
	
	unsigned char i;
	extern const unsigned char BMP1[];
	
	Debug_USART_Config();
	DelayInit();
	I2C_Configuration();	
	OLED_Init();
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
	
		
	printf("\r\n这是一个实验\r\n");

	
  while(1)
	{
		OLED_Fill(0xFF);//全屏点亮
		DelayS(2);
		OLED_Fill(0x00);//全屏灭
		DelayS(2);
		for(i=0;i<5;i++)
		{
			OLED_ShowCN(22+i*16,0,i);//测试显示中文
		}
		DelayS(2);
		OLED_ShowStr(0,3,"HelTec Automation",1);//测试6*8字符
		OLED_ShowStr(0,4,"Hello Tech",2);				//测试8*16字符
		DelayS(2);
		OLED_CLS();//清屏
		OLED_OFF();//测试OLED休眠
		DelayS(2);
		OLED_ON();//测试OLED休眠后唤醒
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//测试BMP位图显示
		DelayS(2);
	}
}



/*********************************************END OF FILE**********************/

