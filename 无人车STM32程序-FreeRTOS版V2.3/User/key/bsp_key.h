#ifndef __KEY_H
#define	__KEY_H

#include "stm32f4xx.h"

//引脚定义
/*******************************************************/
#define KEY1_INT_GPIO_PORT                GPIOA
#define KEY1_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define KEY1_INT_GPIO_PIN                 GPIO_Pin_0
#define KEY1_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE           EXTI_PinSource0
#define KEY1_INT_EXTI_LINE                EXTI_Line0
#define KEY1_INT_EXTI_IRQ                 EXTI0_IRQn

#define KEY1_IRQHandler                   EXTI0_IRQHandler

#define KEY2_INT_GPIO_PORT                GPIOC
#define KEY2_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define KEY2_INT_GPIO_PIN                 GPIO_Pin_13
#define KEY2_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOC
#define KEY2_INT_EXTI_PINSOURCE           EXTI_PinSource13
#define KEY2_INT_EXTI_LINE                EXTI_Line13
#define KEY2_INT_EXTI_IRQ                 EXTI15_10_IRQn

#define KEY2_IRQHandler                   EXTI15_10_IRQHandler

/*******************************************************/

 /** 按键按下标置宏
	* 按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
	* 若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
	*/
#define KEY_ON	1
#define KEY_OFF	0

void EXTI_Key_Config(void);


#endif /* __LED_H */

