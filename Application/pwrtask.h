/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： pwrtask.h
* 文件标识：
* 内容摘要： pwrtask相关函数声明
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
#ifndef __PWRTASK_H
#define __PWRTASK_H
#include "stm32l1xx.h"
#include "MainConfig.h"


void Creat_PWR_Task(void);      //创建低功耗任务
void PWRTaskSuspend(void);      //挂起低功耗任务
void PWRTaskResume(void);       //恢复低功耗任务
uint16_t GetLowPowerCount(void);  //获取低功耗休眠时间计数值 单位秒
void SetLowPowerCount(uint16_t varsec);  //设置低功耗休眠时间  单位秒
void setPwrCount(uint16_t varSec);       //设置varSec秒后无操作进入低功耗
uint16_t getPwrCount(void);              //获取剩余时间(秒)进入低功耗
void sysEntery_Stop(uint16_t varSec);    //进入低功耗STOP MODE
void sysTick_Cmd(FunctionalState NewState);   //滴答时钟关闭


#endif  /* __PWRTASK_H */


































