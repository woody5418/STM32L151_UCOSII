/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： MainConfig.h
* 文件标识：
* 内容摘要： MainConfig相关函数声明
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
#ifndef __MAINCONFIG_H_
#define __MAINCONFIG_H_
#include "stm32l1xx.h"
/*****************************************
 * μc/OS-II库头文件
 *****************************************/
#include "ucos_ii.h"
#include "os_cpu.h"
#include "os_cfg.h"

/*****************************************
 * String函数头文件
 *****************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

/*****************************************
 * Common 头文件
 *****************************************/
#include "usart.h"
#include "common.h"
#include "delay.h"

#include "ucSTKCheck.h"

/* Suppress warning messages */
#if defined(__CC_ARM)
// Suppress warning message: extended constant initialiser used
#pragma diag_suppress 1296
#elif defined(__ICCARM__)
#elif defined(__GNUC__)
#endif

/***********************************     版本设置    **************************************/
#define  STM32_VERSION "Nercita 2020/09/07 @ v1.0"

/***********************************    控制设备宏   **************************************/
#define DEBUG_LOG_PRINTF 1  //开启(1)/关闭(0) 串口log打印 
#if DEBUG_LOG_PRINTF
    #define LOG_LEVEL            LOG_LEVEL_VERBOSE
#else
    #define LOG_LEVEL            LOG_LEVEL_OFF
#endif
/*********************************** 开启 DEBUG 设置 **************************************/
#if (LOG_LEVEL	>= LOG_LEVEL_ERROR)
	#define	LOG_E(format,...)	\
			OS_ENTER_CRITICAL();printf("E: "format,##__VA_ARGS__);printf("\r\n");OS_EXIT_CRITICAL();
#endif

#if (LOG_LEVEL	>= LOG_LEVEL_WARING)
	#define	LOG_W(format,...)	\
			OS_ENTER_CRITICAL();printf("W: "format,##__VA_ARGS__);printf("\r\n");OS_EXIT_CRITICAL();
#endif

#if (LOG_LEVEL	>= LOG_LEVEL_INFO)
	#define	LOG_I(format,...)	\
			OS_ENTER_CRITICAL();printf("I: "format,##__VA_ARGS__);printf("\r\n");OS_EXIT_CRITICAL();
#endif

#if (LOG_LEVEL	>= LOG_LEVEL_DEBUG)
	#define LOG_D(format, ...)   \
			OS_ENTER_CRITICAL();printf("DEBUG:(%s-%d)" format "\r\n", __func__, __LINE__, ##__VA_ARGS__);printf("\r\n");OS_EXIT_CRITICAL(); 
#endif

#if LOG_LEVEL	>= LOG_LEVEL_VERBOSE
	#define	LOG_V(format,...)	\
			OS_ENTER_CRITICAL();printf("V: "format,##__VA_ARGS__);printf("\r\n");OS_EXIT_CRITICAL();
#endif
/*********************************** 任务相关宏（优先e级） **************************************/
#define 	START_TASK_PRIO      		21 		// 开始任务的优先级设置为最低	
#define 	START_STK_SIZE  			128	    //设置任务堆栈大小
extern OS_STK START_TASK_STK[START_STK_SIZE];	//任务堆栈		

#define 	PWR_TASK_PRIO      			20 		//低功耗任务的优先级设置为最低	
#define 	PWR_STK_SIZE  				80	    //设置任务堆栈大小
extern OS_STK PWR_TASK_STK[PWR_STK_SIZE];	    //任务堆栈		

#define 	Runing_State_PRIO      		19 		//LED任务
#define 	Runing_State_STK_SIZE  	    64		//设置任务堆栈大小
extern OS_STK Runing_TASK_STK[Runing_State_STK_SIZE]; //任务堆栈	

#define 	Parm_State_PRIO      		18 		//FLASH存储读取任务
#define 	Parm_STK_SIZE  	    		64		//设置任务堆栈大小
extern OS_STK Parm_TASK_STK[Parm_STK_SIZE]; 	//任务堆栈	
/*********************************** BSP层NVIC优先级 设置 **************************************/


/***********************************   全局调用数组/变量  **************************************/
#define configCONTROLStopMode			15 //单位秒，无操作等待时间

extern volatile uint8_t chipid[13];  //STM32L151 唯一ID 保存数组

#endif  /* __MAINCONFIG_H_ */



































