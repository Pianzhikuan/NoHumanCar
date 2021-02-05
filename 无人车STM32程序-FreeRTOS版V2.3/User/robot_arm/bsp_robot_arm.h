#ifndef __ROBOT_ARM_H
#define	__ROBOT_ARM_H

#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"
#include "./delay/delay.h"
#include "./can/bsp_test_can.h"
#include "./modbus/bsp_modbus.h"

#define FACTOR 				100  //系数 由于python发送时候是拆开发送的，所以需要乘100
//由于传输10对应ASCII中的回车，会有问题，为此增加11的偏移
#define OFFSET 				11  

void sand_Robot_Arm_Info(uint8_t* buf);



#endif /* __ROBOT_ARM_H */
