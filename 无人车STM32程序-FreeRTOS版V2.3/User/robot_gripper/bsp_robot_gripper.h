#ifndef __ROBOT_GRIPPER_H
#define	__ROBOT_GRIPPER_H

#include "stm32f4xx.h"
#include "./robot_arm/bsp_robot_arm.h"

//初始化机械爪
void init_Robot_Gripper(void);
////抓住
//void gripperCatch(void);
////松开
//void gripperPut(void);

#endif /* __ROBOT_GRIPPER_H */

