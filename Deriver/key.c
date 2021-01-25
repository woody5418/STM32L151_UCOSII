/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： key.c
* 文件标识：
* 内容摘要： key 相关函数定义
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
#include "key.h"


/**************************************************************************
* 函数名称: bsp_InitExtiKEY
* 功能描述: 按键初始化函数
* 输入参数: ButtonMode：按键类型 外部中断/按键
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void bsp_InitExtiKEY(ButtonMode_TypeDef ButtonMode)
{
    GPIO_InitTypeDef   GPIO_InitStructure; //定义一个设置IO的结构体
	NVIC_InitTypeDef   NVIC_InitStructure; //定义一个设置NVIC的结构体
	EXTI_InitTypeDef   EXTI_InitStructure; //定义一个设置EXTI的结构体
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);        //使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);     //使能外设配置时钟
	
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;                 //准备设置PA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;               //上拉输入，默认状态是高电平
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;           //上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);                     //设置PA0

    if (ButtonMode == BUTTON_MODE_EXTI)  {
        SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);   //配置KEY1-PA0中断线
        EXTI_InitStructure.EXTI_Line = EXTI_Line0;                    //准备设置外部0中断线
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //外部中断
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //上升沿触发中断
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
        EXTI_Init(&EXTI_InitStructure);                               //设置外部中断0线
        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;              //准备设置外部中断0
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     //抢占优先级0
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            //子优先级1
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能外部中断0通道
        NVIC_Init(&NVIC_InitStructure);                               //设置外部中断0
    }
}

/**************************************************************************
* 函数名称: EXTI0_IRQHandler
* 功能描述: 外部中断0中断程序.
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void EXTI0_IRQHandler(void)
{
    //只能使用硬件延时去抖动，是否需要有待测试
    EXTI_ClearITPendingBit(EXTI_Line0);  //清除外部中断0线上的中断标志位
}












