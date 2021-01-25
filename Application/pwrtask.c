/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： pwrtask.c
* 文件标识：
* 内容摘要： pwrtask相关函数定义
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
#include "pwrtask.h"
#include "app.h"

OS_STK PWR_TASK_STK[PWR_STK_SIZE];

static volatile uint16_t LowPowerCount = 0; //休眠时间
static volatile uint16_t EnteryPWRCount = 0; //无任何操作就倒计时进入低功耗模式

/**************************************************************************
* 函数名称: vPreSTopProcessing
* 功能描述: 进入低功耗模式前需要处理的事情,详见Document说明
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
static void vPreSTopProcessing(void)
{
    //关闭打开的IO 不用的IO配置成模拟输入模式
    LED_Control(0);
    USART_Cmd(USART1, DISABLE);//关闭串口1
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, DISABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, DISABLE);
    bsp_InitExtiKEY(BUTTON_MODE_EXTI);  //初始化外部中断类型按键 用于唤醒CPU
}

/**************************************************************************
* 函数名称: vPostSTopProcessing
* 功能描述: 退出低功耗模式以后需要处理的事情
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
static void vPostSTopProcessing(void)
{
    bsp_Init();//退出低功耗模式以后打开那些被关闭的外设时钟
}

/**************************************************************************
* 函数名称: sysTick_Cmd
* 功能描述: systick控制
* 输入参数: DISABLE/ENABLE 关闭/开启systick中断
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void sysTick_Cmd(FunctionalState NewState)
{
    if (NewState != DISABLE) {
        SysTick_Config(SystemCoreClock / OS_TICKS_PER_SEC); //systick重新初始化 非常重要的，否则跑飞
    } else {
        SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                         0 |
                         SysTick_CTRL_ENABLE_Msk;
    }
}

/**************************************************************************
* 函数名称: sysEntery_Stop
* 功能描述: 进入低功耗停机模式
* 输入参数: varSec：停机时间
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void sysEntery_Stop(uint16_t varSec)
{
    vPreSTopProcessing();     //进入低功耗模式要做的事
    sysTick_Cmd(DISABLE);     //关闭systick避免systick中断唤醒CPU
    RtcWakeUpConfig(varSec);  //设置唤醒时间 单位秒
    PWR_UltraLowPowerCmd(ENABLE);       //启用超低功耗模式
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI); //控制CPU进入stop模式
    /* CPU唤醒后，先停止自动唤醒功能 */
    RTC_WakeUpCmd(DISABLE);
    RCC_HSEConfig(RCC_HSE_ON);  //唤醒后选择外部高速时钟

    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);

    RCC_PLLCmd(ENABLE);         //使能PLL

    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  //选择PLL为系统时钟源

    while (RCC_GetSYSCLKSource() != 0x0C);

    vPostSTopProcessing();      //退出低功耗模式要做的事
    sysTick_Cmd(ENABLE);        //systick重新初始化 非常重要的，否则跑飞
}

/**************************************************************************
* 函数名称: setPwrCount
* 功能描述: 设置varSec秒后进入Stop mode
* 输入参数: varSec：停机时间
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void setPwrCount(uint16_t varSec)
{
    EnteryPWRCount = varSec;
}

/**************************************************************************
* 函数名称: getPwrCount
* 功能描述: 获取还有多少秒开始进入stop mode
* 输入参数: 无
* 输出参数: 剩余的时间
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
uint16_t getPwrCount(void)
{
    return EnteryPWRCount;
}
/**************************************************************************
* 函数名称: PwrTASK_Function
* 功能描述: 任务最低优先级判断是否进入低功耗模式
* 输入参数: pvParameters 是在创建该任务时传递的形参
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void  PwrTASK_Function(void *pvParameters)
{
#if DEBUG_LOG_PRINTF
    OS_CPU_SR cpu_sr = 0;
#endif
    pvParameters = pvParameters;

    while (1) { //15秒没有任何操作就进入低功耗模式
        if (EnteryPWRCount >= configCONTROLStopMode) { 
            EnteryPWRCount = 0;
#if DEBUG_LOG_PRINTF
            /* 检测任务堆栈使用大小的一种方式*/
            TaskStackMonitor();
#endif
            OSSchedLock();      //禁止任务调度
#if DEBUG_LOG_PRINTF
            LOG_I("ENTER STOP MODE...");
#endif
            SetLowPowerCount(10);   //设置10秒以后唤醒
            sysEntery_Stop(LowPowerCount); //进入低功耗
            OSSchedUnlock();//允许任务调度
#if DEBUG_LOG_PRINTF
            LOG_I("EXIT STOP MODE...");
#endif
        }

        EnteryPWRCount++;
        delay_ms(1000);
    }
}

/**************************************************************************
* 函数名称: PWRTaskSuspend
* 功能描述: 挂起低功耗任务
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void PWRTaskSuspend(void)
{
    OSTaskSuspend(PWR_TASK_PRIO);  //挂起运行指示灯任务
}

/**************************************************************************
* 函数名称: PWRTaskResume
* 功能描述: 恢复低功耗任务
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void PWRTaskResume(void)
{
    OSTaskResume(PWR_TASK_PRIO); //恢复运行指示灯任务
}

/**************************************************************************
* 函数名称: GetLowPowerCount
* 功能描述: 获取休眠时间
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
uint16_t GetLowPowerCount(void)
{
    return LowPowerCount;
}

/**************************************************************************
* 函数名称: SetLowPowerCount
* 功能描述: 设置休眠时间
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void SetLowPowerCount(uint16_t varsec)
{
    LowPowerCount = varsec;
}

/**************************************************************************
* 函数名称: Creat_PWR_Task
* 功能描述: 创建低功耗任务函数
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void Creat_PWR_Task(void)
{
    OSTaskCreate(PwrTASK_Function, (void *)0, (OS_STK *)&PWR_TASK_STK[PWR_STK_SIZE - 1], PWR_TASK_PRIO);
}










