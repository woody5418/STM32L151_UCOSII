/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： delay.c
* 文件标识：
* 内容摘要： delay相关函数定义
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
#include "delay.h"
#include "MainConfig.h"


static uint8_t  fac_us = 0;     //us延时倍乘数
static uint16_t fac_ms = 0;     //ms延时倍乘数

/**************************************************************************
* 函数名称: SysTick_Handler
* 功能描述: systick中断服务函数,使用ucos时用到
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
#ifdef OS_CRITICAL_METHOD   //如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
void SysTick_Handler(void)
{
    OSIntEnter();       //进入中断
    OSTimeTick();       //调用ucos的时钟服务程序
    OSIntExit();        //触发任务切换软中断
}
#endif
/**************************************************************************
* 函数名称: delay_init
* 功能描述: 初始化延迟函数
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void delay_init(void)
{
#ifdef OS_CRITICAL_METHOD   //如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
    uint32_t reload;
#endif
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);   //选择外部时钟  HCLK/8
    fac_us = SystemCoreClock / 8000000;                     //为系统时钟的1/8
#ifdef OS_CRITICAL_METHOD                                   //如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
    reload = SystemCoreClock / 8000000;                     //每秒钟的计数次数 单位为K
    reload *= 1000000 / OS_TICKS_PER_SEC;                   //根据OS_TICKS_PER_SEC设定溢出时间
    fac_ms = 1000 / OS_TICKS_PER_SEC; //代表ucos可以延时的最少单位
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  //开启SYSTICK中断
    SysTick->LOAD = reload;                     //每1/OS_TICKS_PER_SEC秒中断一次
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   //开启SYSTICK
#else
    fac_ms = (uint16_t)fac_us * 1000;               //非ucos下,代表每个ms需要的systick时钟数
#endif
}

/**************************************************************************
* 函数名称: delay_us
* 功能描述: 延时nus    nus:0~204522252(最大值即2^32/fac_us@fac_us=168)
* 输入参数: 要延时的us数.
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
#ifdef OS_CRITICAL_METHOD   //使用了ucos
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD; //LOAD的值
    ticks = nus * fac_us;       //需要的节拍数
    tcnt = 0;
    told = SysTick->VAL;        //刚进入时的计数器值

    while (1) {
        tnow = SysTick->VAL;

        if (tnow != told) {
            if (tnow < told)tcnt += told - tnow; //这里注意一下SYSTICK是一个递减的计数器就可以了.
            else tcnt += reload - tnow + told;

            told = tnow;

            if (tcnt >= ticks)break; //时间超过/等于要延迟的时间,则退出.
        }
    };
}
#else
void delay_us(uint32_t nus)
{
    unsigned int temp;                        //定义一个变量待用
    SysTick->LOAD = us * fac_us;             //计数器的重载值，要注意SysTick是倒数计数的
    //SysTick计数器每倒数一个数，要乘以微秒延时系数
    SysTick->VAL = 0x00;                      //清空当前计数器的值
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ; //使能SysTick计数器，重载值加载到计数器中，开始倒数计数

    do {
        temp = SysTick->CTRL;                 //循环读取SysTick状态寄存器，用于判断计时结束与否
    } while (temp & 0x01 && !(temp & (1 << 16))); //等待时间到达

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数器
}
#endif
/**************************************************************************
* 函数名称: delay_ms
* 功能描述: 延时nms    nms:0~65535
* 输入参数: nms:要延时的ms数
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
#ifdef OS_CRITICAL_METHOD   //使用了ucos
void delay_ms(uint16_t nms)
{
    if (OSRunning == true) { //如果os已经在跑了
        if (nms >= fac_ms) { //延时的时间大于ucos的最少时间周期
            OSTimeDly(nms / fac_ms); //ucos延时
        }

        nms %= fac_ms;              //ucos已经无法提供这么小的延时了,采用普通方式延时
    }

    delay_us((uint32_t)(nms * 1000)); //普通方式延时,此时ucos无法启动调度.
}
#else
void delay_ms(uint16_t nms)
{
    //我们首先注意一个问题SysTick时钟计数器是24位的，最大4M频率下，总共能延时4149.3ms，频率再小的话，最大延时时间只会比4149.3大
    //所以我们以4000为界限，小于4000的延时一次计数就行，大于4000的多次计数
    unsigned char i;      //定义一个变量待用
    unsigned int temp;    //定义一个变量待用

    /*-----------if判断，小于4000ms延时的情况，执行if分支------------------*/
    if (ms < 4000) {
        SysTick->LOAD = (unsigned int)ms * fac_ms; //计数器的重载值，要注意SysTick是倒数计数的
        //SysTick计1个数，要乘以毫秒延时系数，就是计数器的重载值
        SysTick->VAL = 0x00;                       //清空当前计数器的值
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ; //使能SysTick计数器，重载值加载到计数器中，开始倒数

        do {
            temp = SysTick->CTRL;                  //循环读取SysTick状态寄存器，判断计时结束与否
        } while (temp & 0x01 && !(temp & (1 << 16))); //等待时间到达

        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    }  //关闭计数器
    /*--------------大于1800ms延时的情况，执行else分支-------------------*/
    else {
        for (i = 0; i < (ms / 4000); i++) {           //除以4000，整数部分，用for循环，每次延时4000ms
            SysTick->LOAD = (unsigned int)4000 * fac_ms; //计数器的重载值，要注意SysTick是倒数计数的
            SysTick->VAL = 0x00;                      //清空当前计数器的值
            SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ; //使能SysTick计数器，重载值加载到计数器中，开始倒数

            do {
                temp = SysTick->CTRL;                 //循环读取SysTick状态寄存器，判断计时结束与否
            } while (temp & 0x01 && !(temp & (1 << 16))); //等待时间到达

            SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; //关闭计数器
        }

        //接下来余数部分的延时
        SysTick->LOAD = (unsigned int)(ms % 4000) * fac_ms; //计数器的重载值，要注意SysTick是倒数计数的
        SysTick->VAL = 0x00;                          //清空当前计数器的值
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;    //使能SysTick计数器，重载值加载到计数器中，开始倒数

        do {
            temp = SysTick->CTRL;                     //循环读取SysTick状态寄存器，判断计时结束与否
        } while (temp & 0x01 && !(temp & (1 << 16)));  //等待时间到达

        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;    //关闭计数器
    }
}
#endif




/**************************************************************************
* 函数名称: delay_xms
* 功能描述: 延时nms,不会引起任务调度
* 输入参数: nms:要延时的ms数
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void delay_Hms(uint32_t nms)
{
    uint32_t i;
	
    for(i = 0; i < nms; i++) delay_us(1000);
}





























