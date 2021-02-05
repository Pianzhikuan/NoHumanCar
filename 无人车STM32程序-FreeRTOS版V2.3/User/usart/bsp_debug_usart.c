#include "./usart/bsp_debug_usart.h"

 /**
  * @brief  串口初始化,工作模式配置。115200 8-N-1
  * @param  USARTx：第几个串口   bound：波特率为多少
  * @retval 无
  */
#if 1  //调试时，若想要关闭串口，可以置为0
 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */  //4bit for 抢占优先级 0bit for 子优先级  即 16个主0个次
  
	/*************************USART1中断配置****************************/
  /* 配置USART1为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* 抢断优先级为7 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  /* 子优先级为0 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
	
	/*************************USART2中断配置****************************/
	/* 配置USART2为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = SCREEN_USART_IRQ;
	/* 抢断优先级为8 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
	 /* 子优先级为0 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
	
	
	/*************************USART3中断配置****************************/
	#if (USE_USART3_Handler == 1)  //使用USART3 中断
	/* 配置USART2为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = CAMERA_USART_IRQ;
	/* 抢断优先级为9 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;
	 /* 子优先级为2 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
	#endif
	
	/*************************UART4中断配置****************************/
	/* 配置UART4为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = HC_UART_IRQ;
	/* 抢断优先级为10 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;
	 /* 子优先级为0 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
	
	/*************************UART5中断配置****************************/
	/* 配置UART5为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = Gripper_UART_IRQ;
	/* 抢断优先级为11 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 11;
	 /* 子优先级为0 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

void Debug_USART_Config(unsigned int bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK,ENABLE);

  /* 使能 USART 时钟 */
  RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
  
  /* GPIO初始化 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN  ;  
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE,DEBUG_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);
  
  /* 配置串DEBUG_USART 模式 */
  /* 波特率设置：DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = bound;
  /* 字长(数据位+校验位)：8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* 停止位：1个停止位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* 校验位选择：不使用校验 */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* 硬件流控制：不使用硬件流 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USART模式控制：同时使能接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* 完成USART初始化配置 */
  USART_Init(DEBUG_USART, &USART_InitStructure); 
	
	/* 嵌套向量中断控制器NVIC配置 */
	NVIC_Configuration();
	
	/* 使能串口接收中断 */
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);
	
  /* 使能串口 */
  USART_Cmd(DEBUG_USART, ENABLE);
}


void SCREEN_USART_Config(unsigned int bound, uint8_t Parity)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(SCREEN_USART_RX_GPIO_CLK|SCREEN_USART_TX_GPIO_CLK,ENABLE);

  /* 使能 USART2 时钟 */  //在APB1上！！
  RCC_APB1PeriphClockCmd(SCREEN_USART_CLK, ENABLE);
  
  /* GPIO初始化 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = SCREEN_USART_TX_PIN  ;  
  GPIO_Init(SCREEN_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = SCREEN_USART_RX_PIN;
  GPIO_Init(SCREEN_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(SCREEN_USART_RX_GPIO_PORT,SCREEN_USART_RX_SOURCE,SCREEN_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(SCREEN_USART_TX_GPIO_PORT,SCREEN_USART_TX_SOURCE,SCREEN_USART_TX_AF);
  
  /* 配置串DEBUG_USART 模式 */
  /* 波特率设置：DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = bound;
  /* 停止位：1个停止位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	
  //奇校验
	if(Parity == 1)
	{
		/* 字长(数据位+校验位)：9 */
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		/* 校验位选择：不使用校验 */
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
	}
	else if(Parity == 2)
	{
		/* 字长(数据位+校验位)：9 */
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		/* 校验位选择：不使用校验 */
		USART_InitStructure.USART_Parity = USART_Parity_Even;
	}
	//无校验
	else if (Parity == 0)
	{
		/* 字长(数据位+校验位)：8 */
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		/* 校验位选择：不使用校验 */
		USART_InitStructure.USART_Parity = USART_Parity_No;
	}
	
	
  /* 硬件流控制：不使用硬件流 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USART模式控制：同时使能接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* 完成USART初始化配置 */
  USART_Init(SCREEN_USART, &USART_InitStructure); 
  
	/* 嵌套向量中断控制器NVIC配置 */
	NVIC_Configuration();
	
	/* 使能串口接收中断 */
	USART_ITConfig(SCREEN_USART, USART_IT_RXNE, ENABLE);
	
  /* 使能串口 */
  USART_Cmd(SCREEN_USART, ENABLE);
}



void CAMERA_USART_Config(unsigned int bound,uint8_t Parity)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	
	//打开GPIO时钟
  RCC_AHB1PeriphClockCmd(CAMERA_USART_RX_GPIO_CLK|CAMERA_USART_TX_GPIO_CLK,ENABLE);

  /* 使能 USART3 时钟 */  //在APB1上！！
  RCC_APB1PeriphClockCmd(CAMERA_USART_CLK, ENABLE);
  
  /* GPIO初始化 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = CAMERA_USART_TX_PIN  ;  
  GPIO_Init(CAMERA_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = CAMERA_USART_RX_PIN;
  GPIO_Init(CAMERA_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(CAMERA_USART_RX_GPIO_PORT,CAMERA_USART_RX_SOURCE,CAMERA_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(CAMERA_USART_TX_GPIO_PORT,CAMERA_USART_TX_SOURCE,CAMERA_USART_TX_AF);
  
  
  /* 配置串DEBUG_USART 模式 */
  /* 波特率设置：DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = bound;
	/* 停止位：1个停止位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	
	//奇校验
	if(Parity == 1)
	{
		/* 字长(数据位+校验位)：9 */
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		/* 校验位选择：不使用校验 */
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
	}
	else if(Parity == 2)
	{
		/* 字长(数据位+校验位)：9 */
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
		/* 校验位选择：不使用校验 */
		USART_InitStructure.USART_Parity = USART_Parity_Even;
	}
	//无校验
	else if (Parity == 0)
	{
		/* 字长(数据位+校验位)：8 */
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		/* 校验位选择：不使用校验 */
		USART_InitStructure.USART_Parity = USART_Parity_No;
	}
	
  /* 硬件流控制：不使用硬件流 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USART模式控制：同时使能接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* 完成USART3初始化配置 */
  USART_Init(CAMERA_USART, &USART_InitStructure); 
  
	#if USE_USART3_Handler 
	/* 嵌套向量中断控制器NVIC配置 */
	NVIC_Configuration();
	
	/* 使能串口接收中断 */
	USART_ITConfig(CAMERA_USART, USART_IT_RXNE, ENABLE);
	#endif
	
  /* 使能串口 */
  USART_Cmd(CAMERA_USART, ENABLE);
}


void HC_UART_Config(unsigned int bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(HC_UART_RX_GPIO_CLK|HC_UART_TX_GPIO_CLK,ENABLE);

  /* 使能 uart4 时钟 */  //在APB1上！！
  RCC_APB1PeriphClockCmd(HC_UART_CLK, ENABLE);
  
  /* GPIO初始化 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = HC_UART_TX_PIN  ;  
  GPIO_Init(HC_UART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = HC_UART_RX_PIN;
  GPIO_Init(HC_UART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(HC_UART_RX_GPIO_PORT,HC_UART_RX_SOURCE,HC_UART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(HC_UART_TX_GPIO_PORT,HC_UART_TX_SOURCE,HC_UART_TX_AF);
  
  /* 配置串DEBUG_USART 模式 */
  /* 波特率设置：DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = bound;
  /* 字长(数据位+校验位)：8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* 停止位：1个停止位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* 校验位选择：不使用校验 */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* 硬件流控制：不使用硬件流 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USART模式控制：同时使能接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* 完成USART初始化配置 */
  USART_Init(HC_UART, &USART_InitStructure); 
  
	/* 嵌套向量中断控制器NVIC配置 */
	NVIC_Configuration();
	
	/* 使能串口接收中断 */
	USART_ITConfig(HC_UART, USART_IT_RXNE, ENABLE);
	
  /* 使能串口 */
  USART_Cmd(HC_UART, ENABLE);
}

void Gripper_UART_Config(unsigned int bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(Gripper_UART_RX_GPIO_CLK|Gripper_UART_TX_GPIO_CLK,ENABLE);

  /* 使能 uart4 时钟 */  //在APB1上！！
  RCC_APB1PeriphClockCmd(Gripper_UART_CLK, ENABLE);
  
  /* GPIO初始化 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = Gripper_UART_TX_PIN  ;  
  GPIO_Init(Gripper_UART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = Gripper_UART_RX_PIN;
  GPIO_Init(Gripper_UART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(Gripper_UART_RX_GPIO_PORT,Gripper_UART_RX_SOURCE,Gripper_UART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(Gripper_UART_TX_GPIO_PORT,Gripper_UART_TX_SOURCE,Gripper_UART_TX_AF);
  
  /* 配置串DEBUG_USART 模式 */
  /* 波特率设置：DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = bound;
  /* 字长(数据位+校验位)：8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* 停止位：1个停止位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* 校验位选择：不使用校验 */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* 硬件流控制：不使用硬件流 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USART模式控制：同时使能接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* 完成USART初始化配置 */
  USART_Init(Gripper_UART, &USART_InitStructure); 
  
	/* 嵌套向量中断控制器NVIC配置 */
	NVIC_Configuration();
	
	/* 使能串口接收中断 */
	USART_ITConfig(Gripper_UART, USART_IT_RXNE, ENABLE);
	
  /* 使能串口 */
  USART_Cmd(Gripper_UART, ENABLE);
}


void BSP_USART_Config(void)
{
	Debug_USART_Config(115200);
//	SCREEN_USART_Config(115200,Parity_No);
	SCREEN_USART_Config(19200,Parity_Odd);//modbus通信
	CAMERA_USART_Config(19200,Parity_Odd);//modbus通信
	delay_us(0x100);
	
	HC_UART_Config(9600);
	Gripper_UART_Config(115200);//控制机械爪  RS485
	
	//Gripper_UART_Config();
//	/* 嵌套向量中断控制器NVIC配置 */
//	NVIC_Configuration();
}


#endif

/*****************  发送一个字符 **********************/
static void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART2 */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/*****************  指定长度（字符个数）的发送字符串 **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, uint8_t *str,uint32_t strlen )
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 两个字节 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}



///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口DEBUG_USART */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}
/*********************************************END OF FILE******************************/
/******************************************************************************************/
/*************************串口相关处理函数*************************************************/

/**********************************************************************
  * @ 函数名  ： judge_Screen_Res
  * @ 功能说明： 串口数据处理USART2 和 UART4 ---PC
  * @ 参数    ： 缓冲区
  * @ 返回值  ： 无
  ********************************************************************/
void judge_Screen_Res(uint8_t* buf,uint8_t num)//串口屏判断
{
	//各索引值所代表的意义：0~2-帧头 2~7-下，x，y，z  8-帧尾
	//
	uint8_t ch;
	ch = buf[num];
	uint8_t ZhuaCatch[14] = {0xFF, 0xFE, 0xFD, 0xFC, 0x01, 0x10, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFB};//（控制夹爪到达第一个位置）//闭合
	uint8_t ZhuaPut[14] = {0xFF, 0xFE, 0xFD, 0xFC, 0x01, 0x10, 0x04, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFB};//（控制夹爪到达第二个位置）//打开
//	printf("ch=%#x\r\n",ch);
//	printf("buf0=%#x\r\n",buf[0]);
//	printf("buf1=%#x\r\n",buf[1]);
//	printf("buf2=%#x\r\n",buf[2]);
//	printf("buf3=%#x\r\n",buf[3]);
//	printf("buf4=%#x\r\n",buf[4]);
//	printf("buf5=%#x\r\n",buf[5]);
//	printf("buf6=%#x\r\n",buf[6]);
//	printf("buf7=%#x\r\n",buf[7]);
//	printf("buf8=%#x\r\n",buf[8]);
	if(buf[0] == 0x5A)//说明是控制小车的
	{
		if(ch == 0x00) //前进
		{
			add_Speed();
			printf("前进\r\n");
		}
		else if(ch == 0x01)//左转
		{
			left_Turn();
			printf("左转\r\n");
		}
		else if(ch == 0x02)//后退
		{
			sub_Speed();
			printf("后退\r\n");
		}
		else if(ch == 0x03)//右转
		{
			right_Turn();
			printf("右转\r\n");
		}
		else if(ch == 0x04)//刹车
		{
			stop_Car();
			printf("刹车\r\n");
		}
		else if(ch == 0x50)//抓紧
		{
			LED3_TOGGLE;
			Usart_SendStr_length( Gripper_UART ,ZhuaCatch , 14 );
			printf("抓紧\r\n");
		}
		else if(ch == 0x0F)//松开
		{
			LED3_TOGGLE;
			Usart_SendStr_length( Gripper_UART ,ZhuaPut , 14 );
			printf("松开\r\n");
		}
		else
		{
			Usart_SendString(DEBUG_USART,(uint8_t*)"选择错误！请重新选择\t\n");
		}
		
		printf("当前速度为：%.2f KM/s\r\n",(Speed_Sum/10.0));
		printf("当前转角为：%.1f°\r\n",(Turn_Sum/SINGLE_ANGLE));
	}
	else if(buf[0] == 0x3D)//说明是控制机械臂的
	{
		printf("modbus发送机械臂控制信息\r\n");
		sand_Robot_Arm_Info(buf);
	}
}






/******************************************************************************************/
/*************************中断接收相关程序*************************************************/
/**********************USART2_IRQHandler begin ********************************************/
uint8_t SCREEN_RX_BUF[SCREEN_REC_LEN];             //USART2接收缓冲,最大USART_REC_LEN个字节
//接收状态               
u8 SCREEN_RX_Over_flag = 0;//USART2接收完毕标记
u8 SCREEN_RX_Num = 0;

//USART2接收数据帧头、帧尾
u8 SCREEN_DATA_BEGIN1  = 0x5A;
u8 SCREEN_DATA_BEGIN2  = 0xA5;
u8 SCREEN_DATA_END = 0x0A;


//串口2中断服务程序，串口接收正确数据串{码头+ID+控制+结束位} 如：#1+1@ 
//USART1_RX_STA作为状态寄存器，15位作为帧尾标志位，14位作为帧头2标志位，13位作为帧头1标志位
void SCREEN_USART_IRQHandler(void)                	
{
	uint8_t ucTemp;
	delay_us(5);
	if(USART_GetITStatus(SCREEN_USART, USART_IT_RXNE) != RESET)      //接收中断
	{
		
		ucTemp = USART_ReceiveData( SCREEN_USART );

			if (SCREEN_RX_Num < sizeof(SCREEN_RX_BUF) && (SCREEN_RX_Over_flag==0))
			{
				SCREEN_RX_BUF[SCREEN_RX_Num++] = ucTemp;
			}
			else
			{
				SCREEN_RX_Num = 0;
			}
			
			/* 简单的通信协议，遇到回车换行符认为1个命令帧，可自行加其它判断实现自定义命令 */
			/* 遇到换行字符，认为接收到一个命令 */
			if (ucTemp == SCREEN_DATA_END)	/* 换行字符 */
			{	
				LED2_ON;//接收到串口屏的数据
				SCREEN_RX_Over_flag =1; //接收完毕
				/*检测到有回车字符就把数据返回给上位机*/
			}
	}
}  



/**********************USART3_IRQHandler begin ********************************************/
uint8_t CAMERA_RX_BUF[CAMERA_REC_LEN];             //USART3接收缓冲,最大USART_REC_LEN个字节
//接收状态               
u8 CAMERA_RX_flag = 0;//USART2接收完毕标记
u8 CAMERA_Start_RX_flag = 0;//USART3开始接收标记
u8 CAMERA_RX_Num = 0;

//USART3接收数据帧头、帧尾
u8 CAMERA_DATA_BEGIN1  = 0x5A;
u8 CAMERA_DATA_BEGIN2  = 0xA5;
u8 CAMERA_DATA_END = 0x0A;


#if USE_USART3_Handler //若打开了USART3的串口中断
////串口3中断服务程序，串口接收正确数据串{码头+ID+控制+结束位} 如：#1+1@ 
////USART1_RX_STA作为状态寄存器，15位作为帧尾标志位，14位作为帧头2标志位，13位作为帧头1标志位
//void CAMERA_USART_IRQHandler(void)                	
//{
//	uint8_t ucTemp;
//	LED3_ON;
//	delay_us(5);
//	if(USART_GetITStatus(CAMERA_USART, USART_IT_RXNE) != RESET)      //确定进入到了USART3的接收中断
//	{
//		ucTemp = USART_ReceiveData( CAMERA_USART ); //接收数据

//			if (CAMERA_RX_Num < sizeof(CAMERA_RX_BUF))
//			{
//				CAMERA_RX_BUF[CAMERA_RX_Num++] = ucTemp;
//			}
//			else
//			{
//				CAMERA_RX_Num = 0;
//			}
//			
//			/* 简单的通信协议，遇到回车换行符认为1个命令帧，可自行加其它判断实现自定义命令 */
//			/* 遇到换行字符，认为接收到一个命令 */
//			if (ucTemp == CAMERA_DATA_END)	/* 换行字符 */
//			{		
//				CAMERA_RX_flag =1; //接收完毕
//				/*检测到有回车字符就把数据返回给上位机*/
//			}
//	}
//} 
/**********************USART3_IRQHandler begin ********************************************/
uint8_t Recive_Buff[8];//原来是8  总共8个字节
//接收状态               
u8 MODBUS_RX_flag = 0;//USART3接收完毕标记
u8 MODBUS_RX_Num = 0;

//USART1接收数据帧头、帧尾
u8 CAMERA_DATA_BEGIN1  = 0x01;

//串口3中断服务程序，串口接收正确数据串{码头+ID+控制+结束位} 如：#1+1@ 
//USART1_RX_STA作为状态寄存器，15位作为帧尾标志位，14位作为帧头2标志位，13位作为帧头1标志位
void CAMERA_USART_IRQHandler(void)                	
{
	uint8_t ucTemp;
	uint16_t crc = 0;
	uint8_t crcH = 0;
	uint8_t crcL = 0;
	LED3_ON;
	delay_us(5);
	if(USART_GetITStatus(CAMERA_USART, USART_IT_RXNE) != RESET)      //接收中断
	{
		ucTemp = USART_ReceiveData( CAMERA_USART );
		USART_SendData(SCREEN_USART,ucTemp); 
		USART_SendData(DEBUG_USART,ucTemp);   //接收到数据就立马发出来
		if (MODBUS_RX_Num < sizeof(Recive_Buff))
		{
			Recive_Buff[MODBUS_RX_Num++] = ucTemp;
			if(MODBUS_RX_Num == 8)
			{
				crc = CalCrcFast(Recive_Buff,6);
				crcH = ((crc>>8)&0xff);
				crcL = (crc&0xff);
				if(Recive_Buff[6]==crcH && Recive_Buff[7]==crcL)
				{
					MODBUS_RX_flag = 1;
					MODBUS_RX_Num = 0;
				}
			}
		}
		else
		{
			MODBUS_RX_Num = 0;
		}
	}
}  

#endif

/**********************USART3_非中断接收函数********************************************/
#if !USE_USART3_Handler  //如果没有打开串口中断
uint8_t get_USART3(void)
{
	uint16_t dead_time = 0;
	/* 一直不停的等待串口输入数据 */
		while (USART_GetFlagStatus(CAMERA_USART, USART_FLAG_RXNE) == RESET & dead_time < 0xFFF)
		{
			dead_time++;
		}

		return (uint8_t)USART_ReceiveData(CAMERA_USART);
}
#endif


/**********************UART4_IRQHandler begin ********************************************/
uint8_t HC_RX_BUF[HC_REC_LEN];             //USART2接收缓冲,最大USART_REC_LEN个字节
//接收状态               
u8 HC_RX_Over_flag = 0;//USART2接收完毕标记
u8 HC_RX_Num = 0;

//USART2接收数据帧头、帧尾
u8 HC_DATA_BEGIN1  = 0x5A;
u8 HC_DATA_BEGIN2  = 0xA5;//暂时未用
u8 HC_DATA_END = 0x0A;


//串口中断服务程序，串口接收正确数据串{码头+ID+控制+结束位} 如：#1+1@ 
//USART1_RX_STA作为状态寄存器，15位作为帧尾标志位，14位作为帧头2标志位，13位作为帧头1标志位
void HC_UART_IRQHandler(void)                	
{
	uint8_t ucTemp;
	
	delay_us(5);
	if(USART_GetITStatus(HC_UART, USART_IT_RXNE) != RESET)      //接收中断
	{
		ucTemp = USART_ReceiveData( HC_UART );

			if (HC_RX_Num < sizeof(HC_RX_BUF) && (HC_RX_Over_flag==0))
			{
				HC_RX_BUF[HC_RX_Num++] = ucTemp;
			}
			else
			{
				HC_RX_Num = 0;
			}
			
			/* 简单的通信协议，遇到回车换行符认为1个命令帧，可自行加其它判断实现自定义命令 */
			/* 遇到换行字符，认为接收到一个命令 */
			if (ucTemp == HC_DATA_END)	/* 换行字符 */
			{	
				LED2_ON;//接收到串口屏的数据
				HC_RX_Over_flag =1; //接收完毕
				/*检测到有回车字符就把数据返回给上位机*/
			}
	}
}  


/**********************UART5_IRQHandler begin ********************************************/
uint8_t Gripper_RX_BUF[Gripper_REC_LEN];             //USART2接收缓冲,最大USART_REC_LEN个字节
//接收状态               
u8 Gripper_RX_Over_flag = 0;//USART2接收完毕标记
u8 Gripper_RX_Num = 0;

//USART2接收数据帧头、帧尾
u8 Gripper_DATA_BEGIN1  = 0xFF;
u8 Gripper_DATA_BEGIN2  = 0xFF;//暂时未用
u8 Gripper_DATA_END = 0x0A;


//串口2中断服务程序，串口接收正确数据串{码头+ID+控制+结束位} 如：#1+1@ 
//USART1_RX_STA作为状态寄存器，15位作为帧尾标志位，14位作为帧头2标志位，13位作为帧头1标志位
void Gripper_UART_IRQHandler(void)                	
{
	uint8_t ucTemp;
	
	delay_us(5);
	if(USART_GetITStatus(Gripper_UART, USART_IT_RXNE) != RESET)      //接收中断
	{
		ucTemp = USART_ReceiveData( Gripper_UART );
		Usart_SendByte(DEBUG_USART,ucTemp);
		LED2_TOGGLE;
//			if (Gripper_RX_Num < sizeof(Gripper_RX_BUF) && (Gripper_RX_Over_flag==0))
//			{
//				Gripper_RX_BUF[Gripper_RX_Num++] = ucTemp;
//			}
//			else
//			{
//				Gripper_RX_Num = 0;
//			}
//			
//			
//			/* 简单的通信协议，遇到回车换行符认为1个命令帧，可自行加其它判断实现自定义命令 */
//			/* 遇到换行字符，认为接收到一个命令 */
//			if (ucTemp == Gripper_DATA_END)	/* 换行字符 */
//			{	
//				LED2_ON;//接收到串口屏的数据
//				Gripper_RX_Over_flag =1; //接收完毕
//				/*检测到有回车字符就把数据返回给上位机*/
//			}
	}
}  
