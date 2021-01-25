/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： key.h
* 文件标识：
* 内容摘要： key相关函数声明
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
#ifndef __KEY_H
#define __KEY_H
#include "MainConfig.h"

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;



#define KEY_SET  PAin(0) //PA1控制KEY2,读取电平状态,可以判断KEY2是否按下

void bsp_InitExtiKEY(ButtonMode_TypeDef ButtonMode);


#endif  /* __KEY_H */

























