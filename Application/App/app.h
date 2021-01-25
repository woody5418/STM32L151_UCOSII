/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： app.h
* 文件标识：
* 内容摘要： app相关函数声明
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
#ifndef __APP_H
#define __APP_H
#include "MainConfig.h"
/*   BSP 驱动头文件  */
#include "led.h"
#include "rtc.h"
#include "key.h"
#include "w25qxx.h"

/*   app 任务头文件  */
#include "ledtask.h"
#include "pwrtask.h"
#include "parmtask.h"


void bsp_Init(void);
void app_Init(void);

#endif  /* __APP_H */



































