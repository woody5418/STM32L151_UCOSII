/************************************************************************
* Copyright (C) Copyright 2014-2015,Nercita,Beijing,China
*
* 文件名称： main.c
* 文件标识：
* 内容摘要： main相关函数定义
* 其它说明： 
* 当前版本： V1.0
* 作    者： woody   QQ：2490006131
* 完成日期： 2020.09.07
*   < STM32L151ZET6 >
            FLASH SIZE  :512Kbytes
            RAM SIZE    :80Kbytes
            EEPROM      :16K
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
* 修改记录2：…
************************************************************************/
#include "MainConfig.h"
#include "app.h"

OS_STK START_TASK_STK[START_STK_SIZE];	//任务堆栈		

/**************************************************************************
* 函数名称: start_task
* 功能描述: 启动任务，也就是最高优先级任务，这里用来创建其他任务
* 输入参数: pvParameters 是在创建该任务时传递的形参
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void start_task(void *pvParameters)
{
	OS_CPU_SR cpu_sr=0;
	
	pvParameters = pvParameters;    //防止编译器报错
	OSStatInit();					//初始化统计任务.这里会延时1秒钟左右	
	OS_ENTER_CRITICAL();			//进入临界区(无法被中断打断)   
	app_Init();						//任务创建函数
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();				//退出临界区(可以被中断打断)
}


/**************************************************************************
* 函数名称: main
* 功能描述: 主函数
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
int main(void)
{
	OS_CPU_SR cpu_sr=0;
	
    /* 保证睡眠模式下调试器继续可以连接使用 */
    DBGMCU_Config(DBGMCU_STOP, ENABLE);   //定版以后可以屏蔽
    /*  由于 ST 固件库的启动文件已经执行了 CPU 系统时钟的初始化，所以不必再次重复配置系统时钟。
        启动文件配置了 CPU 主时钟频率、内部 Flash 访问速度和可选的外部 SRAM FSMC 初始化。
        系统时钟缺省配置为 32MHz ，如果需要更改，可以修改 system_stm32l1xx.c 文件*/
    /* 优先级分组设置为 2 ，可配置 4 级抢占式优先级， 4 级子优先级。*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	bsp_Init();             //硬件平台初始化
	LOG_I(STM32_VERSION);   //版本
	getChipID();           //获取CHIP id
	OSInit();  	 			//初始化UCOSII	
	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();	 			//开始任务
	
	return -1;
}


/***************************************************************************************************/























