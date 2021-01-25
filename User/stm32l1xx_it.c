/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Templates/stm32l1xx_it.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    16-May-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_it.h"
#include "usart.h"
/** @addtogroup Template_Project
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
enum { r0, r1, r2, r3, r12, lr, pc, psr};

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint16_t ADC_InjectedConvertedValueTab[32];
uint32_t Index = 0;

/* Private function prototypes -----------------------------------------------*/
void Hard_Fault_Handler(uint32_t stack[]);
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
/* Private functions ---------------------------------------------------------*/

static void printUsageErrorMsg(uint32_t CFSRValue)
{
    printf("Usage fault: \r\n");
    CFSRValue >>= 16; // right shift to lsb

    if ((CFSRValue & (1 << 9)) != 0) {
        printf("Divide by zero \r\n");
    }

    if ((CFSRValue & (1 << 8)) != 0) {
        printf("Unaligned access \r\n");
    }
}

static void printBusFaultErrorMsg(uint32_t CFSRValue)
{
    printf("Bus fault: \r\n");
    CFSRValue = ((CFSRValue & 0x0000FF00) >> 8); // mask and right shift to lsb
}

static void printMemoryManagementErrorMsg(uint32_t CFSRValue)
{
    printf("Memory Management fault: \r\n");
    CFSRValue &= 0x000000FF; // mask just mem faults
}

static void stackDump(uint32_t stack[])
{
    static  char msg[80];
    sprintf(msg, "R0 = 0x%08x\r\n", stack[r0]);
    printf("%s",msg);
    sprintf(msg, "R1 = 0x%08x\r\n", stack[r1]);
    printf("%s",msg);
    sprintf(msg, "R2 = 0x%08x\r\n", stack[r2]);
    printf("%s",msg);
    sprintf(msg, "R3 = 0x%08x\r\n", stack[r3]);
    printf("%s",msg);
    sprintf(msg, "R12 = 0x%08x\r\n", stack[r12]);
    printf("%s",msg);
    sprintf(msg, "LR = 0x%08x\r\n", stack[lr]);
    printf("%s",msg);
    sprintf(msg, "PC = 0x%08x\r\n", stack[pc]);
    printf("%s",msg);
    sprintf(msg, "PSR = 0x%08x\r\n", stack[psr]);
    printf("%s",msg);
}

void Hard_Fault_Handler(uint32_t stack[])
{
    static char msg[80];
//if((CoreDebug->DHCSR & 0x01) != 0) {
    printf("\r\nIn Hard Fault Handler\r\n");
    sprintf(msg, "SCB->HFSR = 0x%08x\r\n", SCB->HFSR);
    printf("%s",msg);

    if ((SCB->HFSR & (1 << 30)) != 0) {
        printf("Forced Hard Fault\r\n");
        sprintf(msg, "SCB->CFSR = 0x%08x\r\n", SCB->CFSR);
        printf("%s",msg);

        if ((SCB->CFSR & 0xFFFF0000) != 0) {
            printUsageErrorMsg(SCB->CFSR);
        }

        if ((SCB->CFSR & 0xFF00) != 0) {
            printBusFaultErrorMsg(SCB->CFSR);
        }

        if ((SCB->CFSR & 0xFF) != 0) {
            printMemoryManagementErrorMsg(SCB->CFSR);
        }
    }

    stackDump(stack);
    __ASM volatile("BKPT #01");

//}
    while (1);
}
__ASM void HardFault_Handler_a(void)
{
    IMPORT Hard_Fault_Handler
    TST lr, #4
    ITE EQ
    MRSEQ r0, MSP
    MRSNE r0, PSP
    B Hard_Fault_Handler
}

void HardFault_Handler(void)
{
    printf("\r\nIn Hard Fault Handler\r\n");
    HardFault_Handler_a();

    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1) {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1) {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1) {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None

__weak void SVC_Handler(void)
{
}
  */
/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None

__weak void PendSV_Handler(void)
{
}
*/
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None

__weak void SysTick_Handler(void)
{
}
*/
/******************************************************************************/
/*                 STM32L1xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32l1xx_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
