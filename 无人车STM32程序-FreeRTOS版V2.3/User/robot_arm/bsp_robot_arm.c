#include "./robot_arm/bsp_robot_arm.h"


/*****************************************************************
	* @brief  机械臂控制函数。用于给机械臂发送modbus控制信息（数据处理）
  * @param  buf：串口消息缓冲区
  * @retval 无返回值
  ****************************************************************/
void sand_Robot_Arm_Info(uint8_t* buf)
{
	volatile int16_t x=0;
	volatile int16_t y=0;
	volatile int16_t z=0;
	//X/*************************////////////////////////////////////////
	if(buf[2]>>6) //如果第7位为1，则为负数(由于python串口使用的chr()函数限制，不能设置第8位)
	{
		buf[2] = buf[2] & (~((uint8_t)1<<6));
		x = (buf[2]-OFFSET) * FACTOR + (buf[3]-OFFSET);
		x = x*(-1);
		printf("负数x: %d\r\n",x);
	}
	else //为正数
	{
		x = (buf[2]-OFFSET) * FACTOR + (buf[3]-OFFSET);
		printf("正数x: %d\r\n",x);
	}
	
	//Y/***************************/////////////////////////////////////////
	if(buf[4]>>6) //如果第7位为1，则为负数(由于python串口使用的chr()函数限制，不能设置第8位)
	{
		buf[4] = buf[4] & (~((uint8_t)1<<6));
		y = (buf[4]-OFFSET) * FACTOR + (buf[5]-OFFSET);
		y = y*(-1);
		printf("负数y: %d\r\n",y);
	}
	else //为正数
	{
		y = (buf[4]-OFFSET) * FACTOR + (buf[5]-OFFSET);
		printf("正数y: %d\r\n",y);
	}
	
	//Z/**************************/////////////////////////////////////////
	if(buf[6]>>6)//如果第8位为1，则为负数(由于python串口使用的chr()函数限制，不能设置第8位)
	{
		buf[6] = buf[6] & (~((uint8_t)1<<6));
		z = (buf[6]-OFFSET) * FACTOR + (buf[7]-OFFSET);
		z = z*(-1);
		printf("负数z: %d\r\n",z);
	}
	else //为正数
	{
		z = (buf[6]-OFFSET) * FACTOR + (buf[7]-OFFSET);
		printf("正数z: %d\r\n",z);
	}
	
	LED1_TOGGLE;
	//机械臂modbus函数
	//设置全局变量GV1--X
	Send_Msg_toRobot_HoldReg(1,x);//GV1--在示教器中会对标GV212  300mm
	//设置全局变量GV2--Y
	Send_Msg_toRobot_HoldReg(2,y);
	//设置全局变量GV3--Z
	Send_Msg_toRobot_HoldReg(3,z);
	//设置全局变量GV4--标志位100
	Send_Msg_toRobot_HoldReg(4,100);
}
