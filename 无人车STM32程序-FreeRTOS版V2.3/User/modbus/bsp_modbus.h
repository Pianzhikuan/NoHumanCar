#ifndef __MODBUS_H
#define	__MODBUS_H

#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"
#include "./delay/delay.h"

/* 功能码			数据访问
 * 02--0x02   读离散量输入
 * 01--0x01   读线圈
 * 03--0x03   读多个保持寄存器
 * 04--0x04   读输入寄存器
 * 05--0x05   写单个线圈
 * 15--0x0F   写多个线圈
 * 06--0x06   写单个寄存器
 * 16--0x10   写多个寄存器 
 --------自定义通用寄存器M---------
 0x41类似0x01读
 0x42类似0x02写
 */

//写单线圈时使用
#define MODBUS_ON  0xFF00
#define MODBUS_OFF 0x0000
#define MODBUS_ID  0x01   //跟机械臂的设置要求一一对应
//接收状态   
extern uint8_t Recive_Buff[8];//原来是8  总共8个字节
extern u8 MODBUS_RX_flag;//USART2接收完毕标记
extern u8 MODBUS_RX_Num;


//功能函数#################################################################
/* 参  数：
 * puchMsg  ―― const BYTE*，指向待校验数据存储区的首地址
 * usDataLen ―― 待校验数据的字节个数
 * 返回值：WORD型，计算出的CRC值。   */
uint16_t CalCrcFast(const uint8_t* puchMsg , uint16_t usDataLen);

/****************************************************************************/
/**
* @brief  发送数据给机械臂除去32位的全局变量或者线圈，都用这个函数
	* @param  id-站号  fun-功能码 address-地址  data-数据
  * @retval 无
	* 暂时只支持0x05写单线圈和0x06写保持寄存器
  */
void Send_Msg_toRobot(uint8_t id,uint8_t fun,uint16_t address,uint16_t data);
/**
	* @brief  发送数据给机械臂线圈--即功能码0x05
	* @param   address-地址  data-数据
  * @retval 返回数据指针
	* 暂时只支持0x05写单线圈和0x06写保持寄存器
  */
void Send_Msg_toRobot_Coil(uint16_t address,uint16_t data);
/**
* @brief  发送数据给机械臂全局变量（保持寄存器）--即功能码0x06
	* @param  id-站号   address-地址  data-数据
  * @retval 无
	* 暂时只支持0x05写单线圈和0x06写保持寄存器
  */
void Send_Msg_toRobot_HoldReg(uint16_t address,int16_t data);

/****************************************************************************/
//功能宏定义##################################################

//1-运行程序 0x40-按关节坐标增量运动 x041-按基坐标增量运动
#define RUN_Program(x)     Send_Msg_toRobot(0x01,0x06,440,x)
//停止程序
#define STOP_Program       Send_Msg_toRobot(0x01,0x06,441,1)
//暂停程序
#define PAUSE_Program      Send_Msg_toRobot(0x01,0x06,442,1)
//继续程序
#define CONTINUE_Program   Send_Msg_toRobot(0x01,0x06,443,1)
//全轴运行至零位
#define RUN_TO_ZERO        Send_Msg_toRobot(0x01,0x06,445,11)


#endif /* __MODBUS_H */

