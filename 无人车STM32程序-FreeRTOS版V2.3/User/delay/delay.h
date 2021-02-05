#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"

//void SysTick_Init(void);
void delay_us(u32 nTime);
//void delay_ms(u16 nms);
#define delay_ms(x) delay_us(1000*x)	 //单位ms
#define delay_s(x) delay_ms(1000*x)	 //单位s

#endif /* __DELAY_H */
