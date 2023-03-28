/**
 ******************************************************************************
 * @file    main.c
 * @author  fire
 * @version V1.0
 * @date    2015-xx-xx
 * @brief   DHT11��ʪ�ȴ�������ȡ��
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
#include "./usart/bsp_debug_usart.h"
#include "./systick/bsp_SysTick.h"
#include "./DHT11/bsp_dht11.h"
#include "./HC_SR04/HC_SR04.h"
#include "./beep/bsp_beep.h"
#include "./TouchPad/bsp_touchpad.h"
#include "./OLED/OLED_I2C.h"

void DHT11_TIM3_Configuration(void);
void judge_people(void);
void oled_init(void);
void oled_example(void);
void mode_func(void);
void oled_refresh(void);

/* ��¼��ǰģʽ  */
sw_num sw_sign = no;

/* ����������  */
float distance = 0, dt = 0;

/* �ж�dht11������  */
uint32_t dht11_cnt = 0;

/* �ж�hc_sr04������  */
uint32_t hc_sr04_cnt = 0;

/* ��ʪ��ȫ�ֱ���  */
float humi = 0, temp = 0;
unsigned char temp_num[12], humi_num[12];

/**
 * @brief  ������
 * @param  ��
 * @retval ��
 */
int main(void)
{
	/*��ʼ��USART������ģʽΪ 115200 8-N-1*/
	Debug_USART_Config();

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

	/* ��ʼ��OLED */
	oled_init();

	DEBUG("\r\n ***Ұ��F407������ ��ʪ�ȡ�������OLED��ʾʵ��*** \r\n");

	DHT11_TIM3_Configuration();

	DEBUG("\r\n*** Ұ��F407������ 50us��ʱ������ ***\r\n");

	/* ��ʼ������oled��½��� */
	oled_example();

	while (1)
	{
		mode_func();
		//���ݴ�������ɨ��
		if (TPAD_Scan(0))
		{
			if (sw_sign == no)
			{
				DEBUG("\r\n***����DHT11***\r\n");
				TIM_Cmd(TIM3, ENABLE);
				sw_sign = DHT11;
			}
			else if (sw_sign == DHT11)
			{
				DEBUG("\r\n***����HC_SR04***\r\n");
				sw_sign = HC_SR04;
			}
			else if (sw_sign == HC_SR04)
			{
				DEBUG("\r\n***�ر�***\r\n");
				TIM_Cmd(TIM3, DISABLE);
				// �����ʱ�����м�ʱ
				dht11_cnt = 0;
				hc_sr04_cnt = 0;
				sw_sign = no;	
			}
			/* ÿ�ΰ�����Ӧ������ */
			beep_open();
			/* ����oled��Ӧģʽģ�� */
			oled_example();
		}
	}
}

/*
 * ��������TIM3_IRQHandler
 * ����  ���жϷ�������50us����һ��
 * 		   ������ʪ�Ⱥͳ�������������ʱ
 * ����  ����
 * ���  ����
 */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		// 50us ִ�й��ܺ���
		// ...

		if (sw_sign == DHT11)
		{
			dht11_cnt++;
			if (dht11_cnt == 0xFFFF)
				dht11_cnt = 0;
		}

		if (sw_sign == HC_SR04)
		{
			hc_sr04_cnt++;
			if (hc_sr04_cnt == 0xFFFF)
				hc_sr04_cnt = 0;
		}

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

/*
 * ��������judge_people
 * ����  ���ж����Ƿ��ڷ�Χ��  
 * ����  ����
 * ���  ����
 */
void judge_people(void)
{
	static u8 cnt = 0;
	if (dt > 100)  //������һ����
	{
		cnt++;
	}
	else
	{
		cnt = 0;
	}

	DEBUG("\r\n cnt = %d \r\n", cnt);

	if (cnt > 40)  //������һ���������40��
	{
		BEEP_ON;	//����������
	}
	else
	{
		BEEP_OFF;
	}
}

/*
 * ��������mode_func
 * ����  ��ģʽƥ�书��ʵ��ģ��
 * ����  ����
 * ���  ����
 */
void mode_func(void)
{
	if (sw_sign == DHT11)
	{
		/* ��ʼ��2sһ�����ʪ�� */
		if (dht11_cnt >= 40000)
		{
			dht11_cnt = 0;
			DHT11_func();
			oled_refresh();
		}
	}
	else if (sw_sign == HC_SR04)
	{
		/* ��������� 60ms���һ�� */
		if (hc_sr04_cnt >= 1200)
		{
			hc_sr04_cnt = 0;
			/* ��������ֵת���ɾ��� �����ʽ���Լ��Ƶ� �����ֲ��ϵĹ�ʽ��׼ */
			dt = HC_SR04_GetDistance() * 2 / 58;  
			DEBUG("\r\n distance == %.2fcm\r\n", dt);
			//judge_people();
			oled_refresh();
		}
	}
}


/*
 * ��������oled_example
 * ����  ��OLED��ʼ����ҳģ��
 * ����  ����
 * ���  ����
 */
void oled_example(void)
{
	unsigned char i, j;

	if (sw_sign == no)
	{
		OLED_Fill(0x00); // ȫ����
		for (i = 0, j = 12; i < 4; i++, j++)
		{
			OLED_ShowCN(32 + i * 16, 0, j); // ��ʾ���ģ�ģʽѡ��
		}
		OLED_ShowStr(0, 2, "1", 2);
		OLED_ShowStr(0, 5, "2", 2);

		for (i = 0, j = 2; i < 4; i++, j++)
		{
			OLED_ShowCN(16 + i * 16, 2, j); // ��ʾ���ģ��¶�ʪ�ȼ��
		}
		OLED_ShowCN(16 + 4 * 16, 2, 10);
		OLED_ShowCN(16 + 5 * 16, 2, 11);

		for (i = 0, j = 7; i < 5; i++, j++)
		{
			OLED_ShowCN(16 + i * 16, 5, j); // ��ʾ���ģ����������
		}
	}
	else if (sw_sign == DHT11)
	{
		OLED_Fill(0x00); // ȫ����
		for (i = 0; i < 4; i++)
		{
			OLED_ShowCN(0 + i * 16, 0, i); // ��ʾ���ģ���ǰ�¶�
		}
		for (i = 0; i < 2; i++)
		{
			OLED_ShowCN(0 + i * 16, 3, i); // ��ʾ���ģ���ǰ
		}
		OLED_ShowCN(0 + 2 * 16, 3, 4); // ��ʾ���ģ�ʪ��
		OLED_ShowCN(0 + 3 * 16, 3, 5);
		OLED_ShowStr(0 + 4 * 16, 0, ":", 2);
		OLED_ShowStr(0 + 4 * 16, 3, ":", 2);

		OLED_ShowCN(0 + 7 * 16, 0, 6);		   // ��ʾ ��
		OLED_ShowStr(6 * 16 + 5, 3, "%RH", 2); // ʪ�ȷ���
	}
	else if (sw_sign == HC_SR04) 
	{
		OLED_Fill(0x00); // ȫ����

		for (i = 0, j = 7; i < 5; i++, j++)
		{
			OLED_ShowCN(0 + i * 16, 0, j); // ��ʾ���ģ����������
		}
		OLED_ShowStr(0 + 5 * 16, 0, ":", 2);
		OLED_ShowStr(60, 4, "00", 2);
		OLED_ShowStr(6 * 16 + 5, 4, "cm", 2);
	}
}

/*
 * ��������oled_refresh
 * ����  ��OLED����ˢ����ֵ
 * ����  ����
 * ���  ����
 */
void oled_refresh(void)
{

	if (sw_sign == DHT11)
	{
		sprintf((char *)temp_num, "%.1f ", temp);	//�¶�ǿ������תΪ�ַ���������С�����һλ
		sprintf((char *)humi_num, "%d ", (u8)humi); //ʪ��ǿ������תΪ�ַ���������ҪС���㡤
		OLED_ShowStr(5 * 16 - 5, 0, temp_num, 2); // �¶�����
		OLED_ShowStr(5 * 16 - 5, 3, humi_num, 2); // ʪ������
	}
	else if (sw_sign == HC_SR04)
	{
		sprintf((char *)temp_num, "%d ", (u8)dt);  //����������ǿ������תΪ�ַ���
		OLED_ShowStr(60, 4, "   ", 2);			   //ˢ����ֵ3λ������Ļ���⿨��
		OLED_ShowStr(60, 4, temp_num, 2);		   //д����ֵ
	}
}

/*********************************************END OF FILE**********************/
