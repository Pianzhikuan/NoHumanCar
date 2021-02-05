/**
  ******************************************************************************
  * @file    FMC_SDRAM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* 开发板硬件bsp头文件 */
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"
#include "./delay/delay.h"
#include "./key/bsp_key.h" 
#include "./can/bsp_test_can.h"


/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

///**
//  * @brief  This function handles SVCall exception.
//  * @param  None
//  * @retval None
//  */
//void SVC_Handler(void)
//{}

///**
//  * @brief  This function handles PendSV_Handler exception.
//  * @param  None
//  * @retval None
//  */
//void PendSV_Handler(void)
//{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void xPortSysTickHandler(void);
//systick中断服务函数
void SysTick_Handler(void)
{	
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
      {
    #endif  /* INCLUDE_xTaskGetSchedulerState */  
        xPortSysTickHandler();
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      }
    #endif  /* INCLUDE_xTaskGetSchedulerState */
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/


//USART1用于调试	
//uint8_t USART1_RX_BUF[REC_LEN];             //USART1接收缓冲,最大USART_REC_LEN个字节
////接收状态
//u8 USART1_RX_flag = 0;//USART1接收状态标记
//u8 USART1_RX_Num = 0;
	
uint8_t ucTemp1;
	
void DEBUG_USART_IRQHandler(void)
{
	if(USART_GetITStatus(DEBUG_USART,USART_IT_RXNE)!=RESET)
	{		
		ucTemp1 = USART_ReceiveData( DEBUG_USART );
    USART_SendData(DEBUG_USART,ucTemp1);    
	}	 
}	


	
	
/**
  * @}
  */ 
//extern uint8_t car_Left_flag;//按键1左转标志位
//extern uint8_t car_Right_flag;//按键2右转标志位
//extern uint8_t car_Speed_Up;//按键1加速标志位
//extern uint8_t car_Speed_Down;//按键2减速标志位
extern SemaphoreHandle_t BinarySem_Get_Camera_Handle;
void KEY1_IRQHandler(void)
{
	#if USE_SEM_CAMERA //使用二值信号量
	BaseType_t pxHigherPriorityTaskWoken;
	uint32_t ulReturn;
	//进入临界段,临界段可以嵌套
	ulReturn = taskENTER_CRITICAL_FROM_ISR();
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
		//释放二值信号量，发送接收到新数据标志，供前台程序查询
		xSemaphoreGiveFromISR(BinarySem_Get_Camera_Handle,
											   &pxHigherPriorityTaskWoken);
		
		//如果需要的话进行一次任务切换，系统会判断是否需要进行切换
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		LED3_ON;//蓝色 //开始接收摄像头的数据
    //清除中断标志位
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );
	#else
	 //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
		CAMERA_Start_RX_flag = 1;//开始接收OpenMV
		LED3_ON;//蓝色 //开始接收摄像头的数据
    //清除中断标志位
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	} 
	#endif
}


void KEY2_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
		// LED2 取反		
		LED1_TOGGLE;
    //清除中断标志位
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
}
/**
  * @}
  */ 


/**********************can接收************************************/
extern uint8_t rec_flag;
extern CanRxMsg test_RxMsg;  //接收缓冲区
void TEST_CAN_IRQHandler(void)
{
	if(CAN_GetITStatus(TEST_CAN,CAN_IT_FMP0) == SET)
	{
		/*把接收结构体清零*/
		test_RxMsg.StdId = 0x00; //标准ID
		test_RxMsg.ExtId = 0x00;  //扩展ID
		test_RxMsg.IDE = CAN_ID_STD;//初始化为默认标准帧
		test_RxMsg.DLC = 0;   //数据位数
		test_RxMsg.FMI = 0;  //筛选器0  之后应该会变
		
		CAN_Receive(TEST_CAN,CAN_FIFO0,&test_RxMsg); //读取后自动清除
//		/* 比较 ID 是否为 0x1314 */ 可以放到主函数中
//		if ((test_RxMsg.ExtId==0x1314) && (test_RxMsg.IDE==CAN_ID_EXT) && (test_RxMsg.DLC==8) )
//		{
//			rec_flag = 1; //接收成功
//		}
//		else
//		{
//			rec_flag = 0; //接收失败
//		}
		rec_flag = FULL;
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
