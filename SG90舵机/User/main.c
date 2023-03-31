/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   通用定时器PWM输出
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
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;    // 可以根据具体情况更改极性

    // 根据定时器不同通道的编号选择相应的函数
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
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void) 
{	
  /* 初始化通用定时器PWM输出 */
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

