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

/* 记录当前模式  */
sw_num sw_sign = no;

/* 超声波距离  */
float distance = 0, dt = 0;

/* 中断dht11计数器  */
uint32_t dht11_cnt = 0;

/* 中断hc_sr04计数器  */
uint32_t hc_sr04_cnt = 0;

/* 温湿度全局变量  */
float humi = 0, temp = 0;
unsigned char temp_num[12], humi_num[12];

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

	/* 初始化OLED */
	oled_init();

	DEBUG("\r\n ***野火F407开发板 温湿度、超声波OLED显示实验*** \r\n");

	DHT11_TIM3_Configuration();

	DEBUG("\r\n*** 野火F407开发板 50us定时器开启 ***\r\n");

	/* 初始化进入oled登陆面板 */
	oled_example();

	while (1)
	{
		mode_func();
		//电容触摸按键扫描
		if (TPAD_Scan(0))
		{
			if (sw_sign == no)
			{
				DEBUG("\r\n***开启DHT11***\r\n");
				TIM_Cmd(TIM3, ENABLE);
				sw_sign = DHT11;
			}
			else if (sw_sign == DHT11)
			{
				DEBUG("\r\n***开启HC_SR04***\r\n");
				sw_sign = HC_SR04;
			}
			else if (sw_sign == HC_SR04)
			{
				DEBUG("\r\n***关闭***\r\n");
				TIM_Cmd(TIM3, DISABLE);
				// 清除定时器所有计时
				dht11_cnt = 0;
				hc_sr04_cnt = 0;
				sw_sign = no;	
			}
			/* 每次按键响应蜂鸣器 */
			beep_open();
			/* 加载oled对应模式模板 */
			oled_example();
		}
	}
}

/*
 * 函数名：TIM3_IRQHandler
 * 描述  ：中断服务函数，50us进入一次
 * 		   用于温湿度和超声波传感器定时
 * 输入  ：无
 * 输出  ：无
 */
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		// 50us 执行功能函数
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
 * 函数名：judge_people
 * 描述  ：判断人是否在范围内  
 * 输入  ：无
 * 输出  ：无
 */
void judge_people(void)
{
	static u8 cnt = 0;
	if (dt > 100)  //若人在一米外
	{
		cnt++;
	}
	else
	{
		cnt = 0;
	}

	DEBUG("\r\n cnt = %d \r\n", cnt);

	if (cnt > 40)  //若人在一米外计数过40次
	{
		BEEP_ON;	//蜂鸣器报警
	}
	else
	{
		BEEP_OFF;
	}
}

/*
 * 函数名：mode_func
 * 描述  ：模式匹配功能实现模块
 * 输入  ：无
 * 输出  ：无
 */
void mode_func(void)
{
	if (sw_sign == DHT11)
	{
		/* 初始化2s一检测温湿度 */
		if (dht11_cnt >= 40000)
		{
			dht11_cnt = 0;
			DHT11_func();
			oled_refresh();
		}
	}
	else if (sw_sign == HC_SR04)
	{
		/* 超声波检测 60ms检测一次 */
		if (hc_sr04_cnt >= 1200)
		{
			hc_sr04_cnt = 0;
			/* 超声波数值转换成距离 这个公式是自己推的 按照手册上的公式不准 */
			dt = HC_SR04_GetDistance() * 2 / 58;  
			DEBUG("\r\n distance == %.2fcm\r\n", dt);
			//judge_people();
			oled_refresh();
		}
	}
}


/*
 * 函数名：oled_example
 * 描述  ：OLED初始化首页模板
 * 输入  ：无
 * 输出  ：无
 */
void oled_example(void)
{
	unsigned char i, j;

	if (sw_sign == no)
	{
		OLED_Fill(0x00); // 全屏灭
		for (i = 0, j = 12; i < 4; i++, j++)
		{
			OLED_ShowCN(32 + i * 16, 0, j); // 显示中文：模式选择
		}
		OLED_ShowStr(0, 2, "1", 2);
		OLED_ShowStr(0, 5, "2", 2);

		for (i = 0, j = 2; i < 4; i++, j++)
		{
			OLED_ShowCN(16 + i * 16, 2, j); // 显示中文：温度湿度检测
		}
		OLED_ShowCN(16 + 4 * 16, 2, 10);
		OLED_ShowCN(16 + 5 * 16, 2, 11);

		for (i = 0, j = 7; i < 5; i++, j++)
		{
			OLED_ShowCN(16 + i * 16, 5, j); // 显示中文：超声波检测
		}
	}
	else if (sw_sign == DHT11)
	{
		OLED_Fill(0x00); // 全屏灭
		for (i = 0; i < 4; i++)
		{
			OLED_ShowCN(0 + i * 16, 0, i); // 显示中文：当前温度
		}
		for (i = 0; i < 2; i++)
		{
			OLED_ShowCN(0 + i * 16, 3, i); // 显示中文：当前
		}
		OLED_ShowCN(0 + 2 * 16, 3, 4); // 显示中文：湿度
		OLED_ShowCN(0 + 3 * 16, 3, 5);
		OLED_ShowStr(0 + 4 * 16, 0, ":", 2);
		OLED_ShowStr(0 + 4 * 16, 3, ":", 2);

		OLED_ShowCN(0 + 7 * 16, 0, 6);		   // 显示 ℃
		OLED_ShowStr(6 * 16 + 5, 3, "%RH", 2); // 湿度符号
	}
	else if (sw_sign == HC_SR04) 
	{
		OLED_Fill(0x00); // 全屏灭

		for (i = 0, j = 7; i < 5; i++, j++)
		{
			OLED_ShowCN(0 + i * 16, 0, j); // 显示中文：超声波检测
		}
		OLED_ShowStr(0 + 5 * 16, 0, ":", 2);
		OLED_ShowStr(60, 4, "00", 2);
		OLED_ShowStr(6 * 16 + 5, 4, "cm", 2);
	}
}

/*
 * 函数名：oled_refresh
 * 描述  ：OLED用于刷新数值
 * 输入  ：无
 * 输出  ：无
 */
void oled_refresh(void)
{

	if (sw_sign == DHT11)
	{
		sprintf((char *)temp_num, "%.1f ", temp);	//温度强制类型转为字符串，保留小数点后一位
		sprintf((char *)humi_num, "%d ", (u8)humi); //湿度强制类型转为字符串，不需要小数点·
		OLED_ShowStr(5 * 16 - 5, 0, temp_num, 2); // 温度数字
		OLED_ShowStr(5 * 16 - 5, 3, humi_num, 2); // 湿度数字
	}
	else if (sw_sign == HC_SR04)
	{
		sprintf((char *)temp_num, "%d ", (u8)dt);  //超声波距离强制类型转为字符串
		OLED_ShowStr(60, 4, "   ", 2);			   //刷新数值3位数的屏幕以免卡屏
		OLED_ShowStr(60, 4, temp_num, 2);		   //写入数值
	}
}

/*********************************************END OF FILE**********************/
