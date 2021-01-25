/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： rtc.c
* 文件标识：
* 内容摘要： rtc相关函数定义
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
#include "rtc.h"


/**************************************************************************
* 函数名称: Rtc_Init
* 功能描述: RTC初始化
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void Rtc_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); 	//使能PWR时钟
	
    PWR_RTCAccessCmd(ENABLE);  		//取消RTC写保护
    RCC_RTCResetCmd(ENABLE);   		//复位RTC
    RCC_RTCResetCmd(DISABLE);

    RCC_LSEConfig(RCC_LSE_ON); 		//使能LSE  32.768k

    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET); 	//等待LSE就位

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);			//选择RTC时钟源 LSE
    RCC_RTCCLKCmd(ENABLE);							//使能RTC时钟源
    RTC_WaitForSynchro();                           //RTC同步就位
    //EXIT Config
    EXTI_ClearITPendingBit(EXTI_Line20);            //配置EXTI_Line20，上升沿触发，并使能
    EXTI_InitStructure.EXTI_Line = EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    //enable the rtc wakeup interrupt
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;  //配置wakeup  中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    RTC_ITConfig(RTC_IT_WUT, DISABLE);                                           //返回0，表示成功
}
/**************************************************************************
* 函数名称: RtcWakeUpConfig
* 功能描述: RTC周期性唤醒
* 输入参数: sec：周期时间 秒  最大值0xFFFF(65535) 最长等待时间18小时
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void RtcWakeUpConfig(uint16_t sec)
{
    RTC_WakeUpCmd(DISABLE);                                  //先关闭周期唤醒
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);   //选择时钟为外部32.768KHz 16分频  也就是1Hz时钟
    RTC_SetWakeUpCounter(sec - 1);                           //设定wakeup定时器装载数 时间 秒单位 唤醒
    RTC_ITConfig(RTC_IT_WUT, ENABLE);                        //使能wakeup中断
    RTC_WakeUpCmd(ENABLE);                                   //使能周期唤醒
}

/**************************************************************************
* 函数名称: RTC_WKUP_IRQHandler
* 功能描述: RTC wakeup中断
* 输入参数: sec：周期时间 秒  最大值0xFFFF(65535) 最长等待时间18小时
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void RTC_WKUP_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_WUT) != RESET) {
        RTC_ClearITPendingBit(RTC_IT_WUT);  	//清除WUT标志位
        EXTI_ClearITPendingBit(EXTI_Line20); 	//清除EXTI_Line20中断标志
    }
}








