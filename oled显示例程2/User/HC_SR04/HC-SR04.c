#include "stm32f4xx.h"
#include "HC_SR04.h"
#include "./uart/bsp_debug_uart.h"
#include "./dwt_delay/core_delay.h" 

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

#define HC_SR04_DELAY_US(us)  CPU_TS_Tmr_Delay_US(us)
#define HC_SR04_DELAY_MS(ms)  CPU_TS_Tmr_Delay_MS(ms)

extern float distance;

void HC_SR04_GPIO_Configuration(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = TRIG_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIO_PORT, &GPIO_InitStructure);
}

void HC_SR04_TIM_Configuration(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(SystemCoreClock / 1000000) - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_ICInit(TIM4, &TIM_ICInitStructure);

    TIM_Cmd(TIM4, ENABLE);
}

uint32_t HC_SR04_GetDistance(void) {
    GPIO_SetBits(GPIO_PORT, TRIG_PIN);
    HC_SR04_DELAY_US(10);
    GPIO_ResetBits(GPIO_PORT, TRIG_PIN);

    uint32_t start_time = 0;
    uint32_t end_time = 0;
    uint32_t timeout = 0;

    while (GPIO_ReadInputDataBit(GPIO_PORT, ECHO_PIN) == RESET) {
        if (++timeout > 1000000) {
						printf("\r\n ËÀËÀËÀ \r\n");
            return 0xFFFFFFFF;
        }
    }

    start_time = TIM_GetCounter(TIM4);
    timeout = 0;

    while (GPIO_ReadInputDataBit(GPIO_PORT, ECHO_PIN) == SET) {
        if (++timeout > 1000000) {
						printf("\r\n ËÀËÀËÀ \r\n");
            return 0xFFFFFFFF;
        }
    }

    end_time = TIM_GetCounter(TIM4);

    if (end_time > start_time) {
        distance = (end_time - start_time);
    } else {
//      distance = (start_time - end_time);		
				distance = (0xFFFF - start_time + end_time);		
//			printf("\r\n *********************no \r\n");
    }
//		printf("\r\n distance == %f\r\n",distance);
		
    return distance;
}

void HC_SR04_init(void)
{
	HC_SR04_GPIO_Configuration();
	HC_SR04_TIM_Configuration();
}
