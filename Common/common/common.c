/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： common.c
* 文件标识：
* 内容摘要： common相关函数定义
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
#include "common.h"
#include "MainConfig.h"
#include "rtc.h"
#include "app.h"

//STM32L151 唯一ID 保存数组
volatile uint8_t chipid[13];

/**************************************************************************
* 函数名称: GET_STM32_ID
* 功能描述: 获取96位ID
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
static void GET_STM32_ID(uint8_t *idnum)
{
    for (uint8_t i = 0; i < 12; i++) {
        idnum[i] = *(uint8_t *)(CHIPID_STM32L1 + i);
    }
}
/**************************************************************************
* 函数名称: get_chip_id
* 功能描述: 获取CHIP ID保存到全局数组
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void getChipID(void)
{
    GET_STM32_ID((uint8_t *)chipid);                    //获取唯一ID
#if DEBUG_LOG_PRINTF
    printf("CHIPID:");                           //串口打印CHIPID

    for (uint8_t i = 0; i < 12; i++) {
        printf("%02x", chipid[i]);
    }

    printf("\r\n");
#endif
}


////////////////////////////////////////////////////////////////////////////////

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI
void WFI_SET(void)
{
    __ASM volatile("wfi");
}
//关闭所有中断
void INTX_DISABLE(void)
{
    __ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
    __ASM volatile("cpsie i");
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(uint32_t addr)
{
    MSR MSP, r0             //set Main Stack value
    BX r14
}


