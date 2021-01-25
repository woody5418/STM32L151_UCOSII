/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： parmtask.c
* 文件标识：
* 内容摘要： parmtask 相关函数定义
* 其它说明：
* 当前版本：
* 作    者： woody
* 完成日期： 2020.09.07
*
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
* 修改记录2：…
************************************************************************/
#include "parmtask.h"
#include "app.h"

static uint8_t testtimes = 0;
static uint8_t TEXT_Buffer[64];

OS_STK Parm_TASK_STK[Parm_STK_SIZE];

/**************************************************************************
* 函数名称: Parameter_Function
* 功能描述: 写FLASH任务
* 输入参数: pvParameters 是在创建该任务时传递的形参
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void  Parameter_Function(void *pvParameters)
{
    pvParameters = pvParameters;
#if DEBUG_LOG_PRINTF
    OS_CPU_SR cpu_sr = 0;
#endif

    while (1) {
        testtimes++;

        if ((testtimes % 2) == 0) {
            sprintf((char *)TEXT_Buffer, "STM32L151 SPI2 TEST W25Q64 %03d", testtimes);
            LOG_I("%s", (char *)TEXT_Buffer);
            W25QXX_Erase_Sector(0);
            W25QXX_Write((uint8_t *)TEXT_Buffer, 0, sizeof(TEXT_Buffer));
        } else {
            memset(TEXT_Buffer, 0, 64);
            W25QXX_Read(TEXT_Buffer, 0, 30);            //从倒数第100个地址处开始,读出SIZE个字节
#if DEBUG_LOG_PRINTF
            LOG_I("W25QXX Read data : %s", (char *)TEXT_Buffer);
#endif
        }

        delay_ms(1000);         //延时5s
    }
}

/**************************************************************************
* 函数名称: ParmTaskSuspend
* 功能描述: 挂起任务
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void ParmTaskSuspend(void)
{
    OSTaskSuspend(Parm_State_PRIO);  //挂起运行指示灯任务
}

/**************************************************************************
* 函数名称: ParmTaskResume
* 功能描述: 挂起任务
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void ParmTaskResume(void)
{
    OSTaskResume(Parm_State_PRIO); //恢复运行指示灯任务
}

/**************************************************************************
* 函数名称: Creat_Parm_Task
* 功能描述: 创建写flash任务函数
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void Creat_Parm_Task(void)
{
    OSTaskCreate(Parameter_Function, (void *)0, (OS_STK *)&Parm_TASK_STK[Parm_STK_SIZE - 1], Parm_State_PRIO);
}
















