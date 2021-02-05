#include "./can/bsp_test_can.h"

uint8_t mail_box = 0;
uint8_t i = 0;
uint8_t countOf_Brake_On = 2;//每次按刹车，会发送的次数
//每个索引的意思：0-挡位 1~2-转角 3-速度 4-刹车 后面的几位无用
//uint8_t car_Forward[8] = {0x41,0x00,0x00,0x28,0x00,0x00,0x00,0x00};
//uint8_t car_Left[8] = {0x40,0xA8, 0xFD, 0x00, 0x00, 0x00, 0x00, 0x00};
//uint8_t car_Right[8] = {0x40,0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
//uint8_t car_Data[8] = {0x40, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00};//初始化为刹车
Car_Data car_Data = {Gears_N, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00};//Car_Data数据初始化

//const uint8_t Gears_P = 0x40;//P档--index：0
//const uint8_t Gears_D = 0x41;//D档--index：0
//const uint8_t Gears_N = 0x42;//N档--index：0
//const uint8_t Gears_R = 0x43;//R档--index：0

int16_t Turn_Sum = 0;//转弯角度--index：1~2
uint8_t Speed_Sum = 0; //速度0~20km/h----0~200--index：3
const uint8_t car_Brake_On = 0x02;//刹车--index：4
const uint8_t car_Brake_Off = 0x00;//不刹车--index：4

uint8_t rec_flag = EMPTY;//can接收标志位

CanTxMsg test_TxMsg;//can发送寄存器
CanRxMsg test_RxMsg;//can接收寄存器

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = TEST_CAN_IRQ;
  /* 抢断优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级为3 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  TEST_CAN GPIO 配置,工作模式配置回环工作模式，1MHz。
  * @param  无
  * @retval 无
  */
void CAN_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  CAN_InitTypeDef CAN_InitStructure;
	
	//使能GPIO的时钟
  RCC_AHB1PeriphClockCmd(TEST_CAN_RX_GPIO_CLK|TEST_CAN_TX_GPIO_CLK,ENABLE);

  /* 使能 USART 时钟 */
  RCC_APB1PeriphClockCmd(TEST_CAN_CLK, ENABLE);
  
	  
 /* 连接 PXx 到 CANTx_Tx*/
  GPIO_PinAFConfig(TEST_CAN_RX_GPIO_PORT,TEST_CAN_RX_SOURCE,TEST_CAN_RX_AF);

  /*  连接 PXx 到 CANx__Rx*/
  GPIO_PinAFConfig(TEST_CAN_TX_GPIO_PORT,TEST_CAN_TX_SOURCE,TEST_CAN_TX_AF);
	
  /* GPIO初始化 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = TEST_CAN_TX_PIN  ;  
  GPIO_Init(TEST_CAN_RX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = TEST_CAN_RX_PIN;
  GPIO_Init(TEST_CAN_TX_GPIO_PORT, &GPIO_InitStructure);
  
  /* 配置回环模式 */
  /* 波特率设置：TEST_CAN_DIV */
	CAN_InitStructure.CAN_ABOM = ENABLE;//自动的总线关闭管理
	CAN_InitStructure.CAN_TTCM = DISABLE; //时间触发通信模式
	CAN_InitStructure.CAN_AWUM = ENABLE;  //自动唤醒
	CAN_InitStructure.CAN_NART = ENABLE;  //自动重传  报文自动重传（可关闭）
	CAN_InitStructure.CAN_RFLM = DISABLE;  //不锁定FIFO(锁定的话会丢弃新报文，不锁定那新报文就能覆盖旧报文)
	CAN_InitStructure.CAN_TXFP = DISABLE;  //使得ID比较重要，而不是按照顺序处理
	CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack; //双机时改为正常模式CAN_Mode_Normal  回环CAN_Mode_LoopBack
	CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler = TEST_CAN_DIV;  //设置为12分频  ---500KHz
 /* CAN Baudrate = 1 MBps (1MBps 已为 stm32 的 CAN 最高速率) (CAN 时钟频率为 APB 1 = 42 MHz) 
35 ////BTR-BRP 波特率分频器 定义了时间单元的时间长度 42/(1+4+2)/6=1 Mbps*/

  /* 完成USART初始化配置 */
  CAN_Init(TEST_CAN, &CAN_InitStructure); 
	
  /* 嵌套向量中断控制器NVIC配置 */
	NVIC_Configuration();
  
	/* 使能串口接收中断 */   //使能FIFIO0中断
	CAN_ITConfig(TEST_CAN, CAN_IT_FMP0, ENABLE);
	
  /* 使能串口 */
  //USART_Cmd(DEBUG_USART, ENABLE);
}



//初始化筛选器
void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef CAN_Filter_Struct;
	
	CAN_Filter_Struct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0  ;//存储在FIFO0，因为使能了FIFO0中断
	CAN_Filter_Struct.CAN_FilterNumber = 14;  //使用can1时范围是0~13，使用can2时范围是14~27
	CAN_Filter_Struct.CAN_FilterMode = CAN_FilterMode_IdMask;//掩码模式（不是列表模式）
	CAN_Filter_Struct.CAN_FilterScale = CAN_FilterScale_32bit;//扩展模式，所以有32位
	CAN_Filter_Struct.CAN_FilterActivation = ENABLE;//激活
	
	//筛选器工作在16位时，用的时High高16位。而工作在32位的时候就是全部
	CAN_Filter_Struct.CAN_FilterIdHigh = (((uint32_t)0x120 <<3 |CAN_Id_Extended|CAN_RTR_Data)&0xFFFF0000)>>16;
	CAN_Filter_Struct.CAN_FilterIdLow = ((uint32_t)0x120 <<3 |CAN_Id_Extended|CAN_RTR_Data)&0x0000FFFF;
	CAN_Filter_Struct.CAN_FilterMaskIdHigh = 0x0;
	CAN_Filter_Struct.CAN_FilterMaskIdLow = 0x0;  //允许0x1314和0x1315的ID可以进来
	
	CAN_FilterInit(&CAN_Filter_Struct);
}

void car_Init(void)
{
	//can初始化
	CAN_Config();
	CAN_Filter_Config();
	//初始化---小车开始的时候是刹车
	car_Data.Gears = Gears_N;  //之前是0x40
	car_Data.TurnL = 0x00;//角度L
	car_Data.TurnH = 0x00;//角度H
	car_Data.Speed = 0x00;//速度
	car_Data.BrakeOrNot = car_Brake_Off;//一开始先不刹车
	Speed_Sum = 0;
	Turn_Sum = 0;
	
}

/*****************************************CAN 发送相关****************************************/
void sandMassage(Car_Data* const Data)
{
	//设置数据 ----可封装成函数
	test_TxMsg.StdId = 0x0120;
	test_TxMsg.ExtId = 0x00;
	test_TxMsg.IDE = CAN_Id_Standard;
	test_TxMsg.RTR = CAN_RTR_Data;  //用于区分是数据帧还是遥控帧
	test_TxMsg.DLC = 8; //数据字节数
	
	{
		test_TxMsg.Data[0] = Data->Gears;
		printf("%#x  ",Data->Gears);
		test_TxMsg.Data[1] = Data->TurnL;
		test_TxMsg.Data[2] = Data->TurnH;
		test_TxMsg.Data[3] = Data->Speed;
		test_TxMsg.Data[4] = Data->BrakeOrNot;
		printf("%#x\r\n",Data->BrakeOrNot);
		test_TxMsg.Data[5] = Data->Reserved5;
		test_TxMsg.Data[6] = Data->Reserved6;
		test_TxMsg.Data[7] = Data->Reserved7;
	}
	
	//发送数据
	mail_box = CAN_Transmit(TEST_CAN,&test_TxMsg);//发送数据
	
	if(mail_box != CAN_TxStatus_NoMailBox)
	{
		//等待stm32成功发送给CAN收发器
		while((CAN_TransmitStatus(TEST_CAN,mail_box)) != CAN_TxStatus_Ok);
		
		delay_us(1000);//给一定的时间让CAN收发器成功发送到总线
		
//		printf("CAN指令发送成功!\r\n");
	}
	else
	{
		printf("\t\nThere is no mail box!\r\n");
	}
}


void add_Speed(void)
{
	//处于空档、初始P档、前进档时加
	if(car_Data.Gears != Gears_R)
	{
		car_Data.Gears = Gears_D;//前进挡
		car_Data.BrakeOrNot = car_Brake_Off;
		Speed_Sum += SPEED_STEP;
	}
	//处于减速档时减
	else if(car_Data.Gears == Gears_R)
	{
		Speed_Sum -= SPEED_STEP;
	}
	
	if(Speed_Sum > MAX_SPEED)
	{
		Speed_Sum = MAX_SPEED;
	}
	else if(Speed_Sum < (SPEED_STEP-1))
	{
		Speed_Sum = 0x00;
		car_Data.Gears = Gears_N;//回到空档
	}
	car_Data.Speed = Speed_Sum;
}


void sub_Speed(void)
{
	//处于空档、初始P档、减速档时加
	if(car_Data.Gears != Gears_D)
	{
		car_Data.Gears = Gears_R;//前进挡
		car_Data.BrakeOrNot = car_Brake_Off;
		Speed_Sum += SPEED_STEP;
	}
	//处于前进档时减
	else if(car_Data.Gears == Gears_D)
	{
		Speed_Sum -= SPEED_STEP;
	}
	
	if(Speed_Sum > MAX_SPEED)
	{
		Speed_Sum = MAX_SPEED;
	}
	else if(Speed_Sum < (SPEED_STEP-1))
	{
		Speed_Sum = 0x00;
		car_Data.Gears = Gears_N;//回到空档
	}
	car_Data.Speed = Speed_Sum;
}

void left_Turn(void)
{
	Turn_Sum -= TURN_STEP;//按照步进值减
	if(Turn_Sum < -MAX_ANGLE)
	{
		Turn_Sum = -MAX_ANGLE;//最大27°
	}
	else if((Turn_Sum > -(TURN_STEP-100)) && (Turn_Sum < SINGLE_ANGLE)) 
	{
		Turn_Sum = 0;
	}
	car_Data.TurnL = (Turn_Sum&0x00FF);
	car_Data.TurnH = (Turn_Sum&0xFF00)>>8;
}

void right_Turn(void)
{
	Turn_Sum += TURN_STEP;
	if(Turn_Sum > MAX_ANGLE)
	{
		Turn_Sum = MAX_ANGLE;
	}
	else if(Turn_Sum < (TURN_STEP-100)&& (Turn_Sum > (-SINGLE_ANGLE)))
	{
		Turn_Sum = 0;
	}
	car_Data.TurnL = (Turn_Sum&0x00FF);
	car_Data.TurnH = (Turn_Sum&0xFF00)>>8;
}

void stop_Car(void)
{
	car_Data.Gears = Gears_N; //回到空挡   之前是P
	car_Data.Speed = 0x00;//速度
	
	if(car_Data.BrakeOrNot == car_Brake_Off)
		car_Data.BrakeOrNot = car_Brake_On;//刹车
	else
		car_Data.BrakeOrNot = car_Brake_Off;//点第二次就不刹车了
		
	Speed_Sum = 0;//速度为0
}

//void stop_Car(void)
//{
//		car_Data.Gears = Gears_N; //回到空挡   之前是P
//		car_Data.Speed = 0x00;//速度
//	car_Data[4] = car_Brake_On;//刹车
//	Speed_Sum = 0;//速度为0
//	countOf_Brake_On = 2;
//}

