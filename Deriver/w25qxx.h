/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： w25qxx.h
* 文件标识：
* 内容摘要： w25qxx相关函数声明
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
#ifndef __W25QXX_H
#define __W25QXX_H
#include "MainConfig.h"

//W25X系列/Q系列芯片列表	   
//W25Q80  ID  0XEF13
//W25Q16  ID  0XEF14
//W25Q32  ID  0XEF15
//W25Q64  ID  0XEF16	
//W25Q128 ID  0XEF17	
//W25Q256 ID  0XEF18
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9

/*************** SPI GPIO PIN SET START***************/
#define  CS_GPIO_PIN            GPIO_Pin_12
#define  MOSI_GPIO_PIN 			GPIO_Pin_15
#define  MISO_GPIO_PIN 			GPIO_Pin_14
#define  CLK_GPIO_PIN 			GPIO_Pin_13
#define  MOSI_GPIO_PinSource	GPIO_PinSource15
#define  MISO_GPIO_PinSource	GPIO_PinSource14
#define  CLK_GPIO_PinSource		GPIO_PinSource13

#define	 W25QXX_CS PBout(12)  //选中FLASH		
/*************** SPI GPIO PIN SET END  ***************/	

void W25QXX_SetSpeed(uint8_t SpeedSet); //设置SPI速度   
uint8_t W25QXX_ReadWriteByte(uint8_t TxData);//SPI总线读写一个字节

void bsp_InitW25QXX(void);
uint8_t	 W25QXX_ReadSR(void);        	//读取状态寄存器 
void W25QXX_Write_SR(uint8_t sr);  		//写状态寄存器
void W25QXX_Write_Enable(void);  		//写使能 
void W25QXX_Write_Disable(void);	    //写保护
uint16_t  W25QXX_ReadID(void);  	    //读取FLASH ID
void W25QXX_Wait_Busy(void);           	//等待空闲
void W25QXX_PowerDown(void);           	//进入掉电模式
void W25QXX_WAKEUP(void);			  	//唤醒
void W25QXX_Erase_Chip(void);    	  	//整片擦除
void W25QXX_Erase_Sector(uint32_t Dst_Addr);//扇区擦除
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   			//读取FLASH
void W25QXX_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite); 		//写入FLASH
void W25QXX_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);	//无校验写入FLASH
void W25QXX_EraseWrite(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);	//擦除 写FLASH

#endif  /* __W25QXX_H */


























