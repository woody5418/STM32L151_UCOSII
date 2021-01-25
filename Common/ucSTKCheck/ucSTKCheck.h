/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： ucSTKCheck.h
* 文件标识：
* 内容摘要： ucSTKCheck相关函数声明
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
#ifndef __UCSTKCHECK_H
#define __UCSTKCHECK_H
#include "stm32l1xx.h"


//任务堆栈信息
typedef struct {
    uint32_t  StackTopAddr;     //任务栈顶地址（注意：堆栈为向下生长）
    uint32_t  StackSize;        //任务堆栈大小（单位：CPU字长，STM32为4字节，32bit)
    uint8_t   Proi;             //任务的优先级
} TASK_STACK_INFO;


void TaskStackMonitor(void);//任务堆栈监视



#endif   /* __UCSTKCHECK_H */
























