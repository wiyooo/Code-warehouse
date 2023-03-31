/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ͨ�ö�ʱ��PWM���
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
#include "./tim/bsp_general_tim.h"

void delay_ms(uint32_t ms) {
  uint32_t i, j;
  for (i = 0; i < ms; i++) {
    for (j = 0; j < 9000; j++);
  }
}

void update_pwm_pulse(TIM_TypeDef* TIMx, uint16_t new_pulse) {
	
		TIM_SetCounter(TIMx, 0);
		
    TIM_OCInitTypeDef TIM_OCStruct;
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_Pulse = new_pulse;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;    // ���Ը��ݾ���������ļ���

    // ���ݶ�ʱ����ͬͨ���ı��ѡ����Ӧ�ĺ���
    if (TIMx == TIM1) {
        TIM_OC1Init(TIMx, &TIM_OCStruct);
    } else if (TIMx == TIM2) {
        TIM_OC1Init(TIMx, &TIM_OCStruct);
    } else if (TIMx == TIM3) {
        TIM_OC2Init(TIMx, &TIM_OCStruct);
    } else if (TIMx == TIM4) {
        TIM_OC1Init(TIMx, &TIM_OCStruct);
    } else if (TIMx == TIM5) {
        TIM_OC1Init(TIMx, &TIM_OCStruct);
    } else if (TIMx == TIM8) {
        TIM_OC1Init(TIMx, &TIM_OCStruct);
    }
		
}


u8 duty = 5;

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void) 
{	
  /* ��ʼ��ͨ�ö�ʱ��PWM��� */
	TIMx_Configuration();
  
  while(1)
  {    
			
			update_pwm_pulse(TIM2, 5);
			delay_ms(0xfff);
		
			update_pwm_pulse(TIM2, 25);
			delay_ms(0xfff);	
  }
}



/*********************************************END OF FILE**********************/

