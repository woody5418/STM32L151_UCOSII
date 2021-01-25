/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： ucSTKCheck.c
* 文件标识：
* 内容摘要： ucSTKCheck相关函数定义
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
#include "ucSTKCheck.h"
#include "MainConfig.h"

#define TASK_ALLCOUNT   4  //总任务数量

//任务记录
//需要写入要监控的任务参数
const TASK_STACK_INFO cg_TaskBuff[TASK_ALLCOUNT] = {
    {(uint32_t)START_TASK_STK, START_STK_SIZE, START_TASK_PRIO},
	{(uint32_t)PWR_TASK_STK, PWR_STK_SIZE, PWR_TASK_PRIO},
    {(uint32_t)Runing_TASK_STK, Runing_State_STK_SIZE, Runing_State_PRIO},
	{(uint32_t)Parm_TASK_STK, Parm_STK_SIZE, Parm_State_PRIO},
};

/**************************************************************************
* 函数名称: FindFirstNonzeroPosition
* 功能描述: 从堆栈底部往上找到第一个非0位置，返回偏移：字
* 输入参数: pStackBottom：栈底地址、StackSize：任务堆栈大小
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
uint32_t FindFirstNonzeroPosition(uint32_t *pStackBottom, uint32_t StackSize)
{
    uint32_t i = 0;

    for (i = 0; i < StackSize; i ++) {
        if (pStackBottom[i] != 0) break;
    }

    return i;
}

/**************************************************************************
* 函数名称: TaskStackMonitor
* 功能描述: 任务状态监控，打印任务堆栈状态
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void TaskStackMonitor(void)
{
    uint32_t i;
    uint32_t StackBottomAddr;   //栈底地址
    uint32_t StackResidual;     //堆栈剩余
	
#if DEBUG_LOG_PRINTF
    OS_CPU_SR cpu_sr = 0;
    LOG_I("----------------Task status----------------");
#endif
    for (i = 0; i < sizeof(cg_TaskBuff) / sizeof(TASK_STACK_INFO); i ++) {
        StackBottomAddr = cg_TaskBuff[i].StackTopAddr + 4;                                                  //计算栈底地址
        StackResidual = FindFirstNonzeroPosition((uint32_t *)StackBottomAddr, cg_TaskBuff[i].StackSize);    //计算使用量
#if DEBUG_LOG_PRINTF
        LOG_I("|Task ID: %d, Remaining Stack SIZE: %dBYTE, Usage Amount: %d%%", 
				cg_TaskBuff[i].Proi, StackResidual,
				(cg_TaskBuff[i].StackSize - StackResidual) * 100 / cg_TaskBuff[i].StackSize);
#endif
    }
#if DEBUG_LOG_PRINTF
    LOG_I("CPU Usage: %d%%", OSCPUUsage);
    LOG_I("----------------------------------------");
#endif
}


