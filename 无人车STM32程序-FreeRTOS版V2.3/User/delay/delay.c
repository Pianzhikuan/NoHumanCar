#include "./delay/delay.h"


//static __IO u32 TimingDelay;
// 
///**
//  * @brief  启动系统滴答定时器 SysTick
//  * @param  无
//  * @retval 无
//  */
//void SysTick_Init(void)
//{
//	/* SystemFrequency / 1000    1ms中断一次
//	 * SystemFrequency / 100000	 10us中断一次
//	 * SystemFrequency / 1000000 1us中断一次
//	 */
//	
////	if (SysTick_Config(SystemCoreClock / 1000000))
//	if (SysTick_Config(8))
//	{ 
//		/* Capture error */ 
//		while (1);
//	}
//}

///**
//  * @brief   us延时程序,10us为一个单位
//  * @param  
//  *		@arg nTime: Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
//  * @retval  无
//  */
//void delay_us(__IO u32 nTime)
//{ 
//	TimingDelay = nTime;	

//	while(TimingDelay != 0);
//}

///**
//  * @brief  获取节拍程序
//  * @param  无
//  * @retval 无
//  * @attention  在 SysTick 中断函数 SysTick_Handler()调用
//  */
//void TimingDelay_Decrement(void)
//{
//	if (TimingDelay != 0x00)
//	{ 
//		TimingDelay--;
//	}
//}
/*********************************************END OF FILE**********************/

//只能用于裸机系统
//void delay_us(u32 nus)
//{
// u32 temp;
// SysTick->LOAD = nus;
// SysTick->VAL=0X00;//清空计数器
// SysTick->CTRL=0X01;//使能，减到零是无动作，采用外部时钟源
// do
// {
//  temp=SysTick->CTRL;//读取当前倒计数值
// }while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达
//     SysTick->CTRL=0x00; //关闭计数器
//    SysTick->VAL =0X00; //清空计数器
//}

void delay_us(u32 nus)
{
	u32 i,j;
	for( i=0 ;i<nus;i++)
	{
		for( j=0 ; j<168 ; j++);//168MHz
	}
}

