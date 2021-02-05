/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
/* 开发板硬件bsp头文件 */
#include "./led/bsp_led.h"
#include "./key/bsp_key.h" 
#include "./usart/bsp_debug_usart.h"
#include "./delay/delay.h"
#include "./can/bsp_test_can.h"
#include "./robot_arm/bsp_robot_arm.h"
#include "bsp_robot_gripper.h"

/*
LED 			PF6 		 PE7 			PE8
KEY				PA0 	   PC13
USART1		TX:PA9   RX:PA10
USART2    TX:PA2   RX:PA3
USART3    TX:PB10  RX:PB11
UART4     TX:PC10  RX:PC11
UART5     TX:PC12  RX:PD2      
CAN2      TX:PB13  RX:PB12
停车：亮红灯LED1
接收到串口屏的数据：亮绿灯 LED2
按一下KEY1按键，开始接收OpenMV的数据：亮蓝灯 LED3
*/
/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
#define AppTaskCreat_PRIORITY      1    //创建任务 任务优先级
/* SCREEN和HC的任务句柄 */
static TaskHandle_t SCREEN_HC_Task_Handle;
#define SCREEN_HC_Task_PRIORITY    3    //串口屏和HC模块的优先级
///* Gripperjixiez的任务句柄 */
//static TaskHandle_t Gripper_Task_Handle;
//#define Gripper_Task_PRIORITY    4    //Gripper机械爪的优先级
/* CAMERA任务句柄 */
static TaskHandle_t CAMERA_Task_Handle = NULL;
#define CAMERA_Task_PRIORITY       2    //OpenMV的优先级 稍微低一点，万一一直没有接收到数据，会把低优先级的任务饿死
/* CAR_CAN任务句柄 --- 此函数单纯只是发送can指令给小车*/
static TaskHandle_t CAR_CAN_Task_Handle = NULL;
#define CAR_CAN_Task_PRIORITY    5   	  //小车控制相关的优先级


/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */

SemaphoreHandle_t BinarySem_Get_Camera_Handle = NULL; //二值信号量句柄
/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);/* 用于创建任务 */

static void SCREEN_HC_Task(void* pvParameters);/* SCREEN_HC_Task任务实现 */
//static void Gripper_Task(void* pvParameters);/* Gripper_Task任务实现 */
static void CAMERA_Task(void* pvParameters);/* CAMERA_Task任务实现 */
static void CAR_CAN_Task(void* pvParameters);/* CAR_CAN_Task任务实现 */

static void BSP_Init(void);/* 用于初始化板载相关资源 */

/*****************************************************************
  * @brief  主函数
  * @param  无
  * @retval 无
  * @note   第一步：开发板硬件初始化 
            第二步：创建APP应用任务
            第三步：启动FreeRTOS，开始多任务调度
  ****************************************************************/
int main(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

  /* 开发板硬件初始化 */
  BSP_Init();
	
	delay_us(0x100);
	//Usart_SendString(DEBUG_USART,(uint8_t*)"陈建科的USART2串口程序\t\n");
	LED1_ON;//停车亮红灯
	
	printf("这是一个STM32F407开发板-FreeRTOS-无人车项目组程序!\r\n\n");
	printf("\r\n");
   /* 创建AppTaskCreate任务 */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )128,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */
  else
    return -1;  
  
  while(1);   /* 正常不会执行到这里 */ 
	
}



/*****************************************END OF FILE****************************************/

/***********************************************************************
  * @ 函数名  ： AppTaskCreate----不含有循环
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  taskENTER_CRITICAL();           //进入临界区---进入之后就不能被打断了！！！
  
	/* 创建BinarySem */
	BinarySem_Get_Camera_Handle = xSemaphoreCreateBinary();
	if(BinarySem_Get_Camera_Handle != NULL)
		printf("BinarySem_Get_Camera_Handle二值信号量创建成功\r\n");
  /* 创建SCREEN_HC_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )SCREEN_HC_Task, /* 任务入口函数 */
                        (const char*    )"SCREEN_HC_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )SCREEN_HC_Task_PRIORITY,	    /* 任务的优先级2 */
                        (TaskHandle_t*  )&SCREEN_HC_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建SCREEN_HC_Task任务成功!\r\n");
  
	/* 创建CAMERA_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )CAMERA_Task, /* 任务入口函数 */
                        (const char*    )"CAMERA_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )CAMERA_Task_PRIORITY,	    /* 任务的优先级3 */
                        (TaskHandle_t*  )&CAMERA_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建CAMERA_Task任务成功!\r\n");
  
	/* 创建CAR_CAN_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )CAR_CAN_Task, /* 任务入口函数 */
                        (const char*    )"CAR_CAN_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )CAR_CAN_Task_PRIORITY,	    /* 任务的优先级5 */
                        (TaskHandle_t*  )&CAR_CAN_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建CAR_CONTRL_Task任务成功!\r\n");
	printf("\r\n");
	
  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  taskEXIT_CRITICAL();            //退出临界区
}

/**********************************************************************
  * @ 函数名  ： SCREEN_HC_Task
  * @ 功能说明： SCREEN_HC_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void SCREEN_HC_Task(void* parameter)
{	
    while (1)
    {
//			printf("进入SCREEN_HC_Task任务!\r\n");
			//接收到串口屏数据---数据处理
			if(SCREEN_RX_Over_flag == 1)
			{
				printf("收到串口屏指令：\r\n");
//				vTaskSuspendAll();//开启调度锁，挂起调度器，在这期间不会出现任务调度
				//数据处理---仲裁串口屏发送过来的数据
				judge_Screen_Res(SCREEN_RX_BUF,SCREEN_RX_Num-2);
				printf("\r\n");
				//数据处理完成，标识清零
				SCREEN_RX_Num = 0;
				SCREEN_RX_Over_flag = 0;//处理完毕
				vTaskDelay(10);//为了让LED小灯多亮一会
				LED2_OFF; //接收串口屏结束
//				xTaskResumeAll();
			}
			
			//接收到PC端发送的数据---数据处理
			if(HC_RX_Over_flag == 1)
			{
				printf("收到HC指令：\r\n");
//				vTaskSuspendAll();//开启调度锁，挂起调度器，在这期间不会出现任务调度		
				//数据处理---仲裁串口屏发送过来的数据
				judge_Screen_Res(HC_RX_BUF,HC_RX_Num-2);
				printf("\r\n");
				//数据处理完成，标识清零
				HC_RX_Num = 0;
				HC_RX_Over_flag = 0;//处理完毕
				vTaskDelay(10);//为了让LED小灯多亮一会
				LED2_OFF; //接收HC结束
//				xTaskResumeAll();
			}
			//优先级较低，每50毫秒，检测一次有没有指令传过来
			vTaskDelay(50);   /* 延时50个tick */
    }
}




///**********************************************************************
//  * @ 函数名  ： Gripper_Task
//  * @ 功能说明： Gripper_Task任务主体
//  * @ 参数    ：   
//  * @ 返回值  ： 无
//  ********************************************************************/
//static void Gripper_Task(void* parameter)
//{	
//    while (1)
//    {
////			printf("进入Gripper_Task任务!\r\n");
//			
////			//接收到PC端发送的数据---数据处理
////			if(Gripper_RX_Over_flag == 1)
////			{
////				printf("收到HC指令：\r\n");

////				printf("\r\n");
////				//数据处理完成，标识清零
////				Gripper_RX_Num = 0;
////				Gripper_RX_Over_flag = 0;//处理完毕
////				vTaskDelay(10);//为了让LED小灯多亮一会
////				LED2_OFF; //接收HC结束
//////				xTaskResumeAll();
////			}
//			//发送代码
//			Usart_SendStr_length( Gripper_UART ,test , 12 );
//			
//			//优先级较低，每50毫秒，检测一次有没有指令传过来
//			vTaskDelay(50);   /* 延时50个tick */
//    }
//}

/**********************************************************************
  * @ 函数名  ： CAMERA_Task//按下KEY1开始执行
  * @ 功能说明： CAMERA_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
#define DEAD_TIME  0xF0F
static void CAMERA_Task(void* parameter)
{	
	uint16_t dead_time = 0;
	uint8_t ch = 0;
	#if (USE_SEM_CAMERA==1) //若想要使用二值信号量，需要添加宏定义
		BaseType_t xReturn = pdPASS;//定义一个创建信息返回值，默认为pdPASS
	#endif
    while (1)
    {
//			printf("进入CAMERA_Task任务!\r\n");
			#if (USE_SEM_CAMERA==1)
        //按下KEY1按键，开始接收OpenMV的数据
			xReturn = xSemaphoreTake(BinarySem_Get_Camera_Handle,  //二值信号量句柄
															 portMAX_DELAY); //一直阻塞等待
			if(xReturn == pdTRUE)
			#else
			if(CAMERA_Start_RX_flag == 1)
			#endif
			{
//				CAMERA_Start_RX_flag暂时不用，使用二值信号量CPU效率更高
				printf("开始接收OpenMV的信息!\r\n");
			//	printf("将SCREEN_HC任务挂起!\r\n");
				//vTaskSuspend(SCREEN_HC_Task_Handle);//开启调度锁，挂起调度器，在这期间不会出现任务调度
				CAMERA_RX_Num = 0;//再次初始化，以防万一
				
				do //等待帧头过来！！帧头为0xA5
				{
					ch = get_USART3();
					dead_time++;
				}while(ch != 0xA5 & dead_time<DEAD_TIME); 
				
				CAMERA_RX_BUF[CAMERA_RX_Num++] = ch;//将帧头存起来
				//进入循环开始接收数据
				while(1)
				{	
					if( dead_time==DEAD_TIME )
					{
						printf("OpenMV接收数据失败\r\n");
						break;
					}
					else
					{
						dead_time = 0;
					}
					
					/////////////////////////////////
					ch = get_USART3();
					if(CAMERA_RX_Num <=  sizeof(CAMERA_RX_BUF))
					{
						CAMERA_RX_BUF[CAMERA_RX_Num++] = ch;
					}
					else
					{
						CAMERA_RX_Num = 0;
					}
					
					if (ch == 0x0A)	/* 接收到帧尾----0x0A换行字符 */
					{	
						Usart_SendStr_length( DEBUG_USART, CAMERA_RX_BUF, CAMERA_RX_Num ); //--通过USART1将接收到的摄像头数据发送给电脑
						printf("OpenMV数据个数：%d",CAMERA_RX_Num);
						break;
						/*检测到有回车字符就把数据返回给上位机*/
					}
				}
				
					//恢复初始化
				CAMERA_RX_Num = 0;
				dead_time = 0;
				LED3_OFF;//蓝色关闭----处理完成摄像头的数据
				
				#if (USE_SEM_CAMERA==0)  //不使用二值信号量时
				CAMERA_Start_RX_flag = 0;//暂时不用，采用二值信号量
				vTaskDelay(50);   /* 延时50个tick */
				#endif
				//vTaskResume(SCREEN_HC_Task_Handle);//调度锁关闭，可以被调度
			}
    }
}

/**********************************************************************
  * @ 函数名  ： CAR_CAN_Task
  * @ 功能说明： CAR_CAN_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void CAR_CAN_Task(void* parameter)
{  
	/* 用于保存上次时间。调用后系统自动更新 */
	static portTickType PreviousWakeTime;
	/* 设置延时时间，将时间转为节拍数 每70毫秒发送一次指令*/
	const portTickType TimeIncrement = pdMS_TO_TICKS(pdMS_TO_TICKS(10));//也可以直接写50，因为当前配置一个tick为1ms
	/* 获取当前系统时间 */
	PreviousWakeTime = xTaskGetTickCount();
	
    while (1)
    {
			/* 调用绝对延时函数,任务时间间隔为 50 个 tick */
			vTaskDelayUntil( &PreviousWakeTime,TimeIncrement );
			//vTaskDelay(50); 
		//printf("进入CAR_CAN_Task了!\r\n");
		
			/************************can***************************/
			//状态指示灯--速度为0时亮红灯
			if( car_Data.Speed==0 || Speed_Sum==0x00 )
			{
				LED1_ON;//停车亮红灯
				
			}
			else
			{
				LED1_OFF;
			}
			/**************不断发送can数据控制小车***************************************/
			sandMassage(&car_Data);
		}
}

/***********************************************************************
  * @ 函数名  ： BSP_Init
  * @ 功能说明： 板级外设初始化，所有板子上的初始化均可放在这个函数里面
  * @ 参数    ：   
  * @ 返回值  ： 无
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用这个优先级分组，千万不要再分组，切忌。
	 */
	
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
//  /*初始化按键*/
  EXTI_Key_Config();
//	
//	//USART相关初始化
	BSP_USART_Config();
//	delay_us(0x100);
	
	//小车can初始化
	car_Init();
	
	
	//*****************************初始化机械爪！！***************************************
	init_Robot_Gripper();

}

