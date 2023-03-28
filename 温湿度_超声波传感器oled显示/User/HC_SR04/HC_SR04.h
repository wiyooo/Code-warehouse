#ifndef __HC_SR04_H_
#define __HC_SR04_H_


#define TRIG_PIN GPIO_Pin_13
#define ECHO_PIN GPIO_Pin_15
#define GPIO_PORT GPIOA

// Ä£Ê½Ã¶¾Ù
typedef enum {
	no = 0,
	DHT11,
	HC_SR04	
} sw_num;

//typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

void HC_SR04_init(void);
uint32_t HC_SR04_GetDistance(void);

#endif /* HC_SR04 */
