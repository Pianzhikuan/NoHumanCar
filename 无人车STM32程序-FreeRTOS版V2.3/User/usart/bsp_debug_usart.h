#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "./led/bsp_led.h"
#include "./delay/delay.h"
#include "./can/bsp_test_can.h"
#include "./modbus/bsp_modbus.h"
#include "./robot_arm/bsp_robot_arm.h"



//无校验No  奇校验Odd  偶校验Even 
#define Parity_No      0
#define Parity_Odd     1
#define Parity_Even    2


//USART1引脚定义   USB-USART1   TX:PA9   RX:PA10
/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK                         RCC_APB2Periph_USART1

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_RX_PIN                      GPIO_Pin_10
#define DEBUG_USART_RX_AF                       GPIO_AF_USART1
#define DEBUG_USART_RX_SOURCE                   GPIO_PinSource10

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_TX_PIN                      GPIO_Pin_9
#define DEBUG_USART_TX_AF                       GPIO_AF_USART1
#define DEBUG_USART_TX_SOURCE                   GPIO_PinSource9

#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                         USART1_IRQn
/************************************************************/

//USART2引脚定义----串口屏  TX:PA2   RX:PA3
/*******************************************************/
#define SCREEN_USART                             USART2
#define SCREEN_USART_CLK                         RCC_APB1Periph_USART2

#define SCREEN_USART_RX_GPIO_PORT                GPIOA
#define SCREEN_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define SCREEN_USART_RX_PIN                      GPIO_Pin_3
#define SCREEN_USART_RX_AF                       GPIO_AF_USART2
#define SCREEN_USART_RX_SOURCE                   GPIO_PinSource3

#define SCREEN_USART_TX_GPIO_PORT                GPIOA
#define SCREEN_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define SCREEN_USART_TX_PIN                      GPIO_Pin_2
#define SCREEN_USART_TX_AF                       GPIO_AF_USART2
#define SCREEN_USART_TX_SOURCE                   GPIO_PinSource2

#define SCREEN_USART_IRQHandler                  USART2_IRQHandler
#define SCREEN_USART_IRQ                 				 USART2_IRQn
////////////////////////////////////////////////////////////////
#define SCREEN_REC_LEN  		30  	//定义最大接收字节数 30

extern uint8_t SCREEN_RX_BUF[SCREEN_REC_LEN];             //USART2接收缓冲,最大USART_REC_LEN个字节
//接收状态               
extern u8 SCREEN_RX_Over_flag;//USART2接收完毕标记
extern u8 SCREEN_RX_Num;

/************************************************************/

//USART3引脚定义----  TX:PB10   RX:PB11
/*******************************************************/
#define CAMERA_USART                             USART3
#define CAMERA_USART_CLK                         RCC_APB1Periph_USART3

#define CAMERA_USART_RX_GPIO_PORT                GPIOB
#define CAMERA_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define CAMERA_USART_RX_PIN                      GPIO_Pin_11
#define CAMERA_USART_RX_AF                       GPIO_AF_USART3
#define CAMERA_USART_RX_SOURCE                   GPIO_PinSource11

#define CAMERA_USART_TX_GPIO_PORT                GPIOB
#define CAMERA_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define CAMERA_USART_TX_PIN                      GPIO_Pin_10
#define CAMERA_USART_TX_AF                       GPIO_AF_USART3
#define CAMERA_USART_TX_SOURCE                   GPIO_PinSource10

#define CAMERA_USART_IRQHandler                  USART3_IRQHandler
#define CAMERA_USART_IRQ                 				 USART3_IRQn
#define USE_USART3_Handler                       0              //关闭USART3中断
////////////////////////////////////////////////////////////////
#define CAMERA_REC_LEN  		30  	//定义最大接收字节数 30

extern uint8_t CAMERA_RX_BUF[CAMERA_REC_LEN];             //USART2接收缓冲,最大USART_REC_LEN个字节
//接收状态               
extern u8 CAMERA_RX_flag;//USART3接收完毕标记
extern u8 CAMERA_Start_RX_flag;//USART3开始接收标记
extern u8 CAMERA_RX_Num;

/************************************************************/

//UART4引脚定义----串口转无线HC12   TX:PC10  RX:PC11
/*******************************************************/
#define HC_UART                             UART4
#define HC_UART_CLK                         RCC_APB1Periph_UART4

#define HC_UART_RX_GPIO_PORT                GPIOC
#define HC_UART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define HC_UART_RX_PIN                      GPIO_Pin_11
#define HC_UART_RX_AF                       GPIO_AF_UART4
#define HC_UART_RX_SOURCE                   GPIO_PinSource11

#define HC_UART_TX_GPIO_PORT                GPIOC
#define HC_UART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define HC_UART_TX_PIN                      GPIO_Pin_10
#define HC_UART_TX_AF                       GPIO_AF_UART4
#define HC_UART_TX_SOURCE                   GPIO_PinSource10

#define HC_UART_IRQHandler                  UART4_IRQHandler
#define HC_UART_IRQ                 				UART4_IRQn
////////////////////////////////////////////////////////////////
#define HC_REC_LEN  		30  	//定义最大接收字节数 30

extern uint8_t HC_RX_BUF[HC_REC_LEN];             //USART2接收缓冲,最大USART_REC_LEN个字节
//接收状态               
extern u8 HC_RX_Over_flag;//USART2接收完毕标记
extern u8 HC_RX_Num;

/************************************************************/

//UART5引脚定义----转RS485控制机械爪gripper   TX:PC12  RX:PD2
/*******************************************************/
#define Gripper_UART                             UART5
#define Gripper_UART_CLK                         RCC_APB1Periph_UART5

#define Gripper_UART_RX_GPIO_PORT                GPIOD
#define Gripper_UART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define Gripper_UART_RX_PIN                      GPIO_Pin_2
#define Gripper_UART_RX_AF                       GPIO_AF_UART5
#define Gripper_UART_RX_SOURCE                   GPIO_PinSource2

#define Gripper_UART_TX_GPIO_PORT                GPIOC
#define Gripper_UART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define Gripper_UART_TX_PIN                      GPIO_Pin_12
#define Gripper_UART_TX_AF                       GPIO_AF_UART5
#define Gripper_UART_TX_SOURCE                   GPIO_PinSource12

#define Gripper_UART_IRQHandler                  UART5_IRQHandler
#define Gripper_UART_IRQ                 				 UART5_IRQn
////////////////////////////////////////////////////////////////
#define Gripper_REC_LEN  		30  	//定义最大接收字节数 30

extern uint8_t Gripper_RX_BUF[Gripper_REC_LEN];             //USART2接收缓冲,最大USART_REC_LEN个字节
//接收状态               
extern u8 Gripper_RX_Over_flag;//USART2接收完毕标记
extern u8 Gripper_RX_Num;


//接口函数
/*******************************************************/
//初始化
void Debug_USART_Config(unsigned int bound);
void SCREEN_USART_Config(unsigned int bound, uint8_t Parity);
void CAMERA_USART_Config(unsigned int bound, uint8_t Parity);
void HC_UART_Config(unsigned int bound);
void Gripper_UART_Config(unsigned int bound);
void BSP_USART_Config(void);

/*****************  指定长度的发送字符串 **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen );
/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str);
/*****************  发送一个16位数 两个字节 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
/**********************USART3_非中断接收函数********************************************/
uint8_t get_USART3(void);
/**********************串口数据处理函数USART2和UART4********************************************/
void judge_Screen_Res(uint8_t* buf,uint8_t num);//串口屏判断

#endif /* __USART_H */

