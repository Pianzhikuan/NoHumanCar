#ifndef __TEST_CAN_H
#define	__TEST_CAN_H
#include "stm32f4xx.h"
#include <stdio.h>
#include "./delay/delay.h"

typedef enum {
	Gears_P=0x40,   //P档--index：0
	Gears_D=0x41, 	//D档--index：0
	Gears_N=0x42, 	//N档--index：0
	Gears_R=0x43	  //R档--index：0
}Car_Gear;

typedef struct{
	uint8_t Gears;						//挡位0
	uint8_t TurnL;						//转角低位1
	uint8_t TurnH;						//转角高位2
	uint8_t Speed;						//速度3
	uint8_t BrakeOrNot;				//是否刹车4
	uint8_t Reserved5;				//保留5
	uint8_t Reserved6;				//保留6
	uint8_t Reserved7;				//保留7
}Car_Data;

extern Car_Data car_Data;
//引脚定义
/*******************************************************/
#define TEST_CAN                             CAN2
#define TEST_CAN_CLK                         RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2
#define TEST_CAN_DIV                         12  //配置500kHz时的分频系数

#define TEST_CAN_RX_GPIO_PORT                GPIOB
#define TEST_CAN_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define TEST_CAN_RX_PIN                      GPIO_Pin_12
#define TEST_CAN_RX_AF                       GPIO_AF_CAN2
#define TEST_CAN_RX_SOURCE                   GPIO_PinSource12

#define TEST_CAN_TX_GPIO_PORT                GPIOB
#define TEST_CAN_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define TEST_CAN_TX_PIN                      GPIO_Pin_13
#define TEST_CAN_TX_AF                       GPIO_AF_CAN2
#define TEST_CAN_TX_SOURCE                   GPIO_PinSource13

#define TEST_CAN_IRQHandler                  CAN2_RX0_IRQHandler
#define TEST_CAN_IRQ                 				 CAN2_RX0_IRQn
/************************************************************/
//小车控制相关宏定义
/************************************************************/
#define SINGLE_ANGLE 44.4 //每1°角度转换后的数值（1200/27 = 44.4）
#define MAX_ANGLE   1200  //最大角度为27°
#define TURN_STEP  (SINGLE_ANGLE * 3)   //转弯步进--每次步进3°
#define SPEED_STEP  2   //速度步进--每次步进0.2km/h
#define MAX_SPEED   200   //速度上限20km/h
#define EMPTY       0   //等待接收。为空
#define FULL        1

extern uint8_t mail_box;
extern uint8_t i;

//每个索引的意思：0-挡位 1~2-转角 3-速度 4-刹车 后面的几位无用
//uint8_t car_Forward[8] = {0x41,0x00,0x00,0x28,0x00,0x00,0x00,0x00};
//uint8_t car_Left[8] = {0x40,0xA8, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00};
//uint8_t car_Right[8] = {0x40,0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
//extern uint8_t car_Data[8];//初始化为刹车

//extern const uint8_t Gears_P;//P档--index：0
//extern const uint8_t Gears_D;//D档--index：0
//extern const uint8_t Gears_N;//N档--index：0
//extern const uint8_t Gears_R;//R档--index：0

extern int16_t Turn_Sum;//转弯角度--index：1~2
extern uint8_t Speed_Sum; //速度0~20km/h----0~200--index：3
//extern const uint8_t car_Brake_On;//刹车--index：4
//extern const uint8_t car_Brake_Off;//不刹车--index：4

extern uint8_t rec_flag;//can接收标志位
//extern uint8_t car_Left_flag;//按键1左转标志位
//extern uint8_t car_Right_flag;//按键2右转标志位
//extern uint8_t car_Speed_Up;//按键1加速标志位
//extern uint8_t car_Speed_Down;//按键2减速标志位

extern CanTxMsg test_TxMsg;//can发送寄存器
extern CanRxMsg test_RxMsg;//can接收寄存器
/*******************************************************************/

void CAN_Config(void);//CAN配置
void CAN_Filter_Config(void);
void car_Init(void);//整体初始化
//功能函数
void sandMassage(Car_Data* const Date);
void add_Speed(void);//加速
void sub_Speed(void);//减速
void left_Turn(void);//左转
void right_Turn(void);//右转
void stop_Car(void);//刹车














#endif /* __TEST_CAN_H */
