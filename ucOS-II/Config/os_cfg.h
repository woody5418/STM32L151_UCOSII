/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                  uC/OS-II Configuration File for V2.9x
*
*                               (c) Copyright 2005-2012, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
* File    : OS_CFG.H
* By      : Jean J. Labrosse
* Version : V2.92.07
*
* LICENSING TERMS:
* ---------------
*   uC/OS-II is provided in source form for FREE evaluation, for educational use or for peaceful research.
* If you plan on using  uC/OS-II  in a commercial product you need to contact Micrium to properly license
* its use in your product. We provide ALL the source code for your convenience and to help you experience
* uC/OS-II.   The fact that the  source is provided does  NOT  mean that you can use it without  paying a
* licensing fee.
*********************************************************************************************************
*/

#ifndef OS_CFG_H
#define OS_CFG_H



                                       /* ----------------------    各项参数   ----------------------- */
#define OS_APP_HOOKS_EN           0u   /* 使能APP应用的hooks函数 */
#define OS_ARG_CHK_EN             0u   /* Enable (1) or Disable (0) 参数检查                           */
#define OS_CPU_HOOKS_EN           1u   /* 使能 OS_CPU hooks 函数                                       */

#define OS_DEBUG_EN               0u   /* Enable(1) 使能系统调试功能                                   */

#define OS_EVENT_MULTI_EN         0u   /* 使能 OSEventPendMulti() 函数                                 */
#define OS_EVENT_NAME_EN          0u   /* 使能事件 Sem, Mutex, Mbox and Q名称                          */

#define OS_LOWEST_PRIO           63u   /* 这个与我们配置任务优先级有关系，最低优先级任务配置的最大值。 */
									   /* 我们所说的任务优先级关系是：
													 优先级越小,数值越大； 相反，优先级越大，数值越小。*/

#define OS_MAX_EVENTS            10u   /* 事件模块控制最大数(应用程序中)						       */
#define OS_MAX_FLAGS              5u   /* 事件标志组的最大数(应用程序中)	                           */
#define OS_MAX_MEM_PART           0u   /* 内存分区的最大数                                             */
#define OS_MAX_QS                 5u   /* 消息队列的最大数目                                           */
#define OS_MAX_TASKS             10u   /* 应用程序(任务)的最大数                                       */

#define OS_SCHED_LOCK_EN          1u   /* 使能 OSSchedLock() 和 OSSchedUnlock()函数                    */

#define OS_TICK_STEP_EN           1u   /* 使能tick stepping                                            */
#define OS_TICKS_PER_SEC        1000u   /* 每秒的滴答数                                                */


                                       /* ---------------------  任务堆栈大小   ---------------------- */
#define OS_TASK_TMR_STK_SIZE    128u   /* 定时器任务堆栈大小         (# of OS_STK wide entries)        */
#define OS_TASK_STAT_STK_SIZE   128u   /* 统计任务堆栈大小           (# of OS_STK wide entries)        */
#define OS_TASK_IDLE_STK_SIZE   128u   /* 空闲任务堆栈大小           (# of OS_STK wide entries)        */


                                       /* ---------------------     任务管理    ---------------------- */
#define OS_TASK_CHANGE_PRIO_EN    1u   /*     使用 OSTaskChangePrio()   --------------任务优先级切换   */
#define OS_TASK_CREATE_EN         1u   /*     使用 OSTaskCreate()       --------------创建任务         */
#define OS_TASK_CREATE_EXT_EN     1u   /*     使用 OSTaskCreateExt()    --------------创建扩展版本任务 */
#define OS_TASK_DEL_EN            1u   /*     使用 OSTaskDel()          --------------删除任务         */
#define OS_TASK_NAME_EN           1u   /*     使用 OSTaskNameGet()和OSTaskNameSet() ---Get/Set任务名称 */
#define OS_TASK_PROFILE_EN        1u   /*     包含 OS_TCB 性能分析                                     */
#define OS_TASK_QUERY_EN          1u   /*     使用 OSTaskQuery()        --------------查询任务(信息)   */
#define OS_TASK_REG_TBL_SIZE      1u   /*     任务数组变量的大小(#of INT32U entries)                   */
#define OS_TASK_STAT_EN           1u   /*     Enable (1) or Disable(0) 使能统计任务                    */
#define OS_TASK_STAT_STK_CHK_EN   1u   /*     使能检查任务堆栈                                         */
#define OS_TASK_SUSPEND_EN        1u   /*     使用 OSTaskSuspend() 和 OSTaskResume()  ---任务挂起/恢复 */
#define OS_TASK_SW_HOOK_EN        1u   /*     使用 OSTaskSwHook()       --------------任务切换HOOK函数 */


                                       /* -----------------------   事件标志  ------------------------ */
#define OS_FLAG_EN                1u   /* Enable (1) or Disable (0) 使能OS_FLAG事件标志所有功能        */
#define OS_FLAG_ACCEPT_EN         1u   /*     使用 OSFlagAccept()      ------检查(获取)事件状态        */
#define OS_FLAG_DEL_EN            1u   /*     使用 OSFlagDel()         ------删除事件                  */
#define OS_FLAG_NAME_EN           1u   /*     使能事件标志名称                                         */
#define OS_FLAG_QUERY_EN          1u   /*     使用 OSFlagQuery()       ------查询事件                  */
#define OS_FLAG_WAIT_CLR_EN       1u   /* 使用事件等待清除功能                                         */
#define OS_FLAGS_NBITS           16u   /* 定义 OS_FLAGS 数据类型 type (8, 16 or 32)            */


                                       /* --------------------     消息邮箱      --------------------- */
#define OS_MBOX_EN                1u   /* Enable (1) or Disable (0) code generation for MAILBOXES      */
#define OS_MBOX_ACCEPT_EN         1u   /*     Include code for OSMboxAccept()                          */
#define OS_MBOX_DEL_EN            1u   /*     Include code for OSMboxDel()                             */
#define OS_MBOX_PEND_ABORT_EN     1u   /*     Include code for OSMboxPendAbort()                       */
#define OS_MBOX_POST_EN           1u   /*     Include code for OSMboxPost()                            */
#define OS_MBOX_POST_OPT_EN       1u   /*     Include code for OSMboxPostOpt()                         */
#define OS_MBOX_QUERY_EN          1u   /*     Include code for OSMboxQuery()                           */


                                       /* ---------------------     内存管理      -------------------- */
#define OS_MEM_EN                 1u   /* Enable (1) or Disable (0) code generation for MEMORY MANAGER */
#define OS_MEM_NAME_EN            1u   /*     Enable memory partition names                            */
#define OS_MEM_QUERY_EN           1u   /*     Include code for OSMemQuery()                            */


                                       /* ----------------         互斥信号量          --------------- */
#define OS_MUTEX_EN               1u   /* Enable (1) or Disable (0) code generation for MUTEX          */
#define OS_MUTEX_ACCEPT_EN        1u   /*     Include code for OSMutexAccept()                         */
#define OS_MUTEX_DEL_EN           1u   /*     Include code for OSMutexDel()                            */
#define OS_MUTEX_QUERY_EN         1u   /*     Include code for OSMutexQuery()                          */


                                       /* ---------------------- 消息队列管理 ---------------------- */
#define OS_Q_EN                   1u   /* Enable (1) or Disable (0) code generation for QUEUES         */
#define OS_Q_ACCEPT_EN            1u   /*     Include code for OSQAccept()                             */
#define OS_Q_DEL_EN               1u   /*     Include code for OSQDel()                                */
#define OS_Q_FLUSH_EN             1u   /*     Include code for OSQFlush()                              */
#define OS_Q_PEND_ABORT_EN        1u   /*     Include code for OSQPendAbort()                          */
#define OS_Q_POST_EN              1u   /*     Include code for OSQPost()                               */
#define OS_Q_POST_FRONT_EN        1u   /*     Include code for OSQPostFront()                          */
#define OS_Q_POST_OPT_EN          1u   /*     Include code for OSQPostOpt()                            */
#define OS_Q_QUERY_EN             1u   /*     Include code for OSQQuery()                              */


                                       /* ------------------------  信号管理  ------------------------ */
#define OS_SEM_EN                 1u   /* Enable (1) or Disable (0) code generation for SEMAPHORES     */
#define OS_SEM_ACCEPT_EN          1u   /*    Include code for OSSemAccept()                            */
#define OS_SEM_DEL_EN             1u   /*    Include code for OSSemDel()                               */
#define OS_SEM_PEND_ABORT_EN      1u   /*    Include code for OSSemPendAbort()                         */
#define OS_SEM_QUERY_EN           1u   /*    Include code for OSSemQuery()                             */
#define OS_SEM_SET_EN             1u   /*    Include code for OSSemSet()                               */


                                       /* --------------------- TIME MANAGEMENT ---------------------- */
#define OS_TIME_DLY_HMSM_EN       1u   /*     Include code for OSTimeDlyHMSM()                         */
#define OS_TIME_DLY_RESUME_EN     1u   /*     Include code for OSTimeDlyResume()                       */
#define OS_TIME_GET_SET_EN        1u   /*     Include code for OSTimeGet() and OSTimeSet()             */
#define OS_TIME_TICK_HOOK_EN      1u   /*     Include code for OSTimeTickHook()                        */


                                       /* --------------------- TIMER MANAGEMENT --------------------- */
#define OS_TMR_EN                 0u   /* Enable (1) or Disable (0) code generation for TIMERS         */
#define OS_TMR_CFG_MAX           16u   /*     Maximum number of timers                                 */
#define OS_TMR_CFG_NAME_EN        1u   /*     Determine timer names                                    */
#define OS_TMR_CFG_WHEEL_SIZE     8u   /*     Size of timer wheel (#Spokes)                            */
#define OS_TMR_CFG_TICKS_PER_SEC 10u   /*     Rate at which timer management task runs (Hz)            */

#endif
