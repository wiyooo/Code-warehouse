/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ���ڽӷ����ԣ����ڽ��յ����ݺ����ϻش���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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

/* ��¼��������  */
sw_num sw_sign = no;

/* ����������  */
float distance = 0 , dt = 0;

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
	
	unsigned char i;
	extern const unsigned char BMP1[];
	
	Debug_USART_Config();
	DelayInit();
	I2C_Configuration();	
	OLED_Init();
	/* ��ʼ��ϵͳ��ʱ�� */
	SysTick_Init();
		/* DHT11��ʼ�� */
	DHT11_GPIO_Config();
	
	/* ��ʼ�����ݰ��� */
	TPAD_Init();
	
	/* ��ʼ�������� */
	BEEP_GPIO_Config();	
	
	/* ��ʼ�������� */
	HC_SR04_init();
	
		
	printf("\r\n����һ��ʵ��\r\n");

	
  while(1)
	{
		OLED_Fill(0xFF);//ȫ������
		DelayS(2);
		OLED_Fill(0x00);//ȫ����
		DelayS(2);
		for(i=0;i<5;i++)
		{
			OLED_ShowCN(22+i*16,0,i);//������ʾ����
		}
		DelayS(2);
		OLED_ShowStr(0,3,"HelTec Automation",1);//����6*8�ַ�
		OLED_ShowStr(0,4,"Hello Tech",2);				//����8*16�ַ�
		DelayS(2);
		OLED_CLS();//����
		OLED_OFF();//����OLED����
		DelayS(2);
		OLED_ON();//����OLED���ߺ���
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
		DelayS(2);
	}
}



/*********************************************END OF FILE**********************/

