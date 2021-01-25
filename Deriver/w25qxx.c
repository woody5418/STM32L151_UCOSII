/************************************************************************
* Copyright (C) 2020 Nercita
*
* 文件名称： w25qxx.c
* 文件标识：
* 内容摘要： w25qxx相关函数定义
* 其它说明： 每页256字节，扇区擦除每次可以擦除16页，块擦除每次可以擦除256页，
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
#include "w25qxx.h"


uint8_t W25QXX_BUF[4096];

/**************************************************************************
* 函数名称: LED_Init
* 功能描述: LED I/O初始化函数
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void bsp_InitW25QXX(void)
{
	uint16_t W25QXX_TYPE;

#if DEBUG_LOG_PRINTF
    OS_CPU_SR cpu_sr = 0;
#endif
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
	
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = CLK_GPIO_PIN | MISO_GPIO_PIN | MOSI_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = CS_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    GPIO_PinAFConfig(GPIOB, MOSI_GPIO_PinSource, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, MISO_GPIO_PinSource, GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB, CLK_GPIO_PinSource, GPIO_AF_SPI2);
	
    GPIO_SetBits(GPIOB, CLK_GPIO_PIN | MISO_GPIO_PIN | MOSI_GPIO_PIN | CS_GPIO_PIN); //PB13/14/15上拉
	
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; //串行同步时钟的空闲状态为高电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//定义波特率预分频的值:波特率预分频值为256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC值计算的多项式
	
    SPI_Init(SPI2, &SPI_InitStructure); //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
    SPI_Cmd(SPI2, ENABLE);
	
    W25QXX_ReadWriteByte(0xff);//启动传输
    W25QXX_SetSpeed(SPI_BaudRatePrescaler_2);//设置时钟,高速模式
    W25QXX_TYPE = W25QXX_ReadID(); //读取FLASH ID.
#if DEBUG_LOG_PRINTF

    switch (W25QXX_TYPE) {
    case W25Q80:
        LOG_I("W25Q80 ID = 0x%04x", W25QXX_TYPE);
        break;

    case W25Q16:
        LOG_I("W25Q16 ID = 0x%04x", W25QXX_TYPE);
        break;

    case W25Q32:
        LOG_I("W25Q32 ID = 0x%04x", W25QXX_TYPE);
        break;

    case W25Q64:
        LOG_I("W25Q64 ID = 0x%04x", W25QXX_TYPE);
        break;

    case W25Q128:
        LOG_I("W25Q128 ID = 0x%04x", W25QXX_TYPE);
        break;

    default:
        LOG_I("W25QXX ID Read ERR...");
        break;
    }

#endif
}

/**************************************************************************
* 函数名称: W25QXX_ReadSR
* 功能描述: 读取W25QXX的状态寄存器
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 返回状态
* BIT7  6   5   4   3   2   1   0
* SPR   RV  TB BP2 BP1 BP0 WEL BUSY
* SPR:默认0,状态寄存器保护位,配合WP使用
* TB,BP2,BP1,BP0:FLASH区域写保护设置
* WEL:写使能锁定
* BUSY:忙标记位(1,忙;0,空闲)
* 默认:0x00
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
uint8_t W25QXX_ReadSR(void)
{
    uint8_t byte = 0;
    W25QXX_CS = 0;                          //使能器件
    W25QXX_ReadWriteByte(W25X_ReadStatusReg);    //发送读取状态寄存器命令
    byte = W25QXX_ReadWriteByte(0Xff);           //读取一个字节
    W25QXX_CS = 1;                          //取消片选
    return byte;
}

/**************************************************************************
* 函数名称: W25QXX_Write_SR
* 功能描述: 写W25QXX状态寄存器
* 输入参数: 只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void W25QXX_Write_SR(uint8_t sr)
{
    W25QXX_CS = 0;                           //使能器件
    W25QXX_ReadWriteByte(W25X_WriteStatusReg);    //发送写取状态寄存器命令
    W25QXX_ReadWriteByte(sr);                     //写入一个字节
    W25QXX_CS = 1;                           //取消片选
}

/**************************************************************************
* 函数名称: W25QXX_Write_Enable
* 功能描述: W25QXX写使能  ，将WEL置位
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void W25QXX_Write_Enable(void)
{
    W25QXX_CS = 0;                          //使能器件
    W25QXX_ReadWriteByte(W25X_WriteEnable);      //发送写使能
    W25QXX_CS = 1;                          //取消片选
}

/**************************************************************************
* 函数名称: W25QXX_Write_Disable
* 功能描述: W25QXX写禁止  ，将WEL清零
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void W25QXX_Write_Disable(void)
{
    W25QXX_CS = 0;                          //使能器件
    W25QXX_ReadWriteByte(W25X_WriteDisable);     //发送写禁止指令
    W25QXX_CS = 1;                          //取消片选
}

/**************************************************************************
* 函数名称: W25QXX_ReadID
* 功能描述: 读取芯片ID
* 输入参数: 无
* 输出参数: 16bit ID
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
uint16_t W25QXX_ReadID(void)
{
    uint16_t Temp = 0;
    W25QXX_CS = 0;
    W25QXX_ReadWriteByte(0x90);//发送读取ID命令
    W25QXX_ReadWriteByte(0x00);
    W25QXX_ReadWriteByte(0x00);
    W25QXX_ReadWriteByte(0x00);
    Temp |= W25QXX_ReadWriteByte(0xFF) << 8;
    Temp |= W25QXX_ReadWriteByte(0xFF);
    W25QXX_CS = 1;
    return Temp;
}

/**************************************************************************
* 函数名称: W25QXX_Wait_Busy
* 功能描述: 等待空闲
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void W25QXX_Wait_Busy(void)
{
    while ((W25QXX_ReadSR() & 0x01) == 0x01); // 等待BUSY位清空
}

/**************************************************************************
* 函数名称: W25QXX_PowerDown
* 功能描述: 进入掉电模式
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void W25QXX_PowerDown(void)
{
    W25QXX_CS = 0;                          //使能器件
    W25QXX_ReadWriteByte(W25X_PowerDown);   //发送掉电命令
    W25QXX_CS = 1;                          //取消片选
    delay_us(3);                            //等待TPD
}

/**************************************************************************
* 函数名称: W25QXX_WAKEUP
* 功能描述: 唤醒
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void W25QXX_WAKEUP(void)
{
    W25QXX_CS = 0;                          //使能器件
    W25QXX_ReadWriteByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB
    W25QXX_CS = 1;                          //取消片选
    delay_us(3);                            //等待TRES1
}

/**************************************************************************
* 函数名称: W25QXX_Erase_Chip
* 功能描述: 擦除整个芯片，整片擦除时间:
            W25X16:25s
            W25X32:40s
            W25X64:40s
            等待时间超长...
* 输入参数: 无
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void W25QXX_Erase_Chip(void)
{
    W25QXX_Write_Enable();                  //SET WEL
    W25QXX_Wait_Busy();
    W25QXX_CS = 0;                          //使能器件
    W25QXX_ReadWriteByte(W25X_ChipErase);   //发送片擦除命令
    W25QXX_CS = 1;                          //取消片选
    W25QXX_Wait_Busy();                     //等待芯片擦除结束
}

/**************************************************************************
* 函数名称: W25QXX_Erase_Sector
* 功能描述: 擦除一个扇区的最少时间:150ms，一个扇区是4KB
* 输入参数: Dst_Addr:扇区地址
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void W25QXX_Erase_Sector(uint32_t Dst_Addr)
{
    Dst_Addr *= 4096;
    W25QXX_Write_Enable();                  //SET WEL
    W25QXX_Wait_Busy();
    W25QXX_CS = 0;                          //使能器件
    W25QXX_ReadWriteByte(W25X_SectorErase);      //发送扇区擦除指令
    W25QXX_ReadWriteByte((uint8_t)((Dst_Addr) >> 16)); //发送24bit地址
    W25QXX_ReadWriteByte((uint8_t)((Dst_Addr) >> 8));
    W25QXX_ReadWriteByte((uint8_t)Dst_Addr);
    W25QXX_CS = 1;                          //取消片选
    W25QXX_Wait_Busy();                     //等待擦除完成
}

/**************************************************************************
* 函数名称: W25QXX_Read
* 功能描述: 在指定地址开始读取指定长度的数据
* 输入参数: pBuffer:数据存储区、ReadAddr:开始读取的地址(24bit)、
            NumByteToRead:要读取的字节数(最大65535)
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void W25QXX_Read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    uint16_t i;
	
    W25QXX_CS = 0;                          		//使能器件
    W25QXX_ReadWriteByte(W25X_ReadData);         	//发送读取命令
    W25QXX_ReadWriteByte((uint8_t)((ReadAddr) >> 16)); //发送24bit地址
    W25QXX_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
    W25QXX_ReadWriteByte((uint8_t)ReadAddr);

    for (i = 0; i < NumByteToRead; i++) {
        pBuffer[i] = W25QXX_ReadWriteByte(0XFF); 	//循环读数
    }

    W25QXX_CS = 1;                          		//取消片选
}


/**************************************************************************
* 函数名称: W25QXX_Write
* 功能描述: 在指定地址开始写入最大256字节的数据，SPI在一页(0~65535)内写入少于256个字节的数据
* 输入参数: pBuffer:数据存储区、WriteAddr:开始读取的地址(24bit)、
            NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void W25QXX_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t i;
    W25QXX_Write_Enable();                  //SET WEL
    W25QXX_CS = 0;                          //使能器件
    W25QXX_ReadWriteByte(W25X_PageProgram);      //发送写页命令
    W25QXX_ReadWriteByte((uint8_t)((WriteAddr) >> 16)); //发送24bit地址
    W25QXX_ReadWriteByte((uint8_t)((WriteAddr) >> 8));
    W25QXX_ReadWriteByte((uint8_t)WriteAddr);

    for (i = 0; i < NumByteToWrite; i++){
		W25QXX_ReadWriteByte(pBuffer[i]); //循环写数
	}

    W25QXX_CS = 1;                          //取消片选
    W25QXX_Wait_Busy();                     //等待写入结束
}

/**************************************************************************
* 函数名称: W25QXX_Write_NoCheck
* 功能描述: 无检验写SPI FLASH，必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
            具有自动换页功能 ，在指定地址开始写入指定长度的数据,但是要确保地址不越界!
* 输入参数: pBuffer:数据存储区、WriteAddr:开始读取的地址(24bit)、
            NumByteToWrite:要写入的字节数(最大65535)
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void W25QXX_Write_NoCheck(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t pageremain;
    pageremain = 256 - WriteAddr % 256; //单页剩余的字节数

    if (NumByteToWrite <= pageremain)pageremain = NumByteToWrite; //不大于256个字节

    while (1) {
        W25QXX_Write(pBuffer, WriteAddr, pageremain);

        if (NumByteToWrite == pageremain){
			break; //写入结束了
		}
        else { //NumByteToWrite>pageremain
            pBuffer += pageremain;
            WriteAddr += pageremain;
            NumByteToWrite -= pageremain;         //减去已经写入了的字节数

            if (NumByteToWrite > 256)pageremain = 256; //一次可以写入256个字节
            else pageremain = NumByteToWrite;     //不够256个字节了
        }
    };
}

/**************************************************************************
* 函数名称: W25QXX_EraseWrite
* 功能描述: 在指定地址开始写入指定长度的数据，该函数带擦除操作!
* 输入参数: pBuffer:数据存储区、WriteAddr:开始读取的地址(24bit)、
            NumByteToWrite:要写入的字节数(最大65535)
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void W25QXX_EraseWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;
	
    secpos = WriteAddr / 4096; //扇区地址 0~511 for w25x16
    secoff = WriteAddr % 4096; //在扇区内的偏移
    secremain = 4096 - secoff; //扇区剩余空间大小

    if (NumByteToWrite <= secremain)secremain = NumByteToWrite; //不大于4096个字节

    while (1) {
        W25QXX_Read(W25QXX_BUF, secpos * 4096, 4096); //读出整个扇区的内容

        for (i = 0; i < secremain; i++) { //校验数据
            if (W25QXX_BUF[secoff + i] != 0XFF)break; //需要擦除
        }

        if (i < secremain) { //需要擦除
            W25QXX_Erase_Sector(secpos);//擦除这个扇区

            for (i = 0; i < secremain; i++) { //复制
                W25QXX_BUF[i + secoff] = pBuffer[i];
            }

            W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096); //写入整个扇区
        } else W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain); //写已经擦除了的,直接写入扇区剩余区间.

        if (NumByteToWrite == secremain)break; //写入结束了
        else { //写入未结束
            secpos++;//扇区地址增1
            secoff = 0; //偏移位置为0
            pBuffer += secremain; //指针偏移
            WriteAddr += secremain; //写地址偏移
            NumByteToWrite -= secremain;            //字节数递减

            if (NumByteToWrite > 4096)secremain = 4096; //下一个扇区还是写不完
            else secremain = NumByteToWrite;        //下一个扇区可以写完了
        }
    };
}

/*************************************W25QXX DRIVER START*************************************/
/**************************************************************************
* 函数名称: W25QXX_SetSpeed
* 功能描述: SPI 速度设置函数
* 输入参数: SpeedSet:
            SPI_BaudRatePrescaler_2   2分频   (SPI 16M@sys 32M)
            SPI_BaudRatePrescaler_8   8分频   (SPI 4M@sys 32M)
            SPI_BaudRatePrescaler_16  16分频  (SPI 2M@sys 32M)
            SPI_BaudRatePrescaler_256 256分频 (SPI 125K@sys 32M)
* 输出参数: 无
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
void W25QXX_SetSpeed(uint8_t SpeedSet)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    SPI2->CR1 &= 0XFFC7;
    SPI2->CR1 |= SpeedSet;  //设置W25QXX速度
    SPI_Cmd(SPI2, ENABLE);
}

/**************************************************************************
* 函数名称: W25QXX_ReadWriteByte
* 功能描述: 读写一个字节
* 输入参数: TxData:要写入的字节
* 输出参数: 读取到的字节
* 返 回 值: 无
* -----------------------------------------------
* 2020/09/07       V1.0      woody         创建
**************************************************************************/
uint8_t W25QXX_ReadWriteByte(uint8_t TxData)
{
    uint8_t retry = 0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) { //检查指定的SPI标志位设置与否:发送缓存空标志位
        retry++;

        if (retry > 200)return 0;
    }

    SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
    retry = 0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) { //检查指定的SPI标志位设置与否:接受缓存非空标志位
        retry++;

        if (retry > 200)return 0;
    }

    return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据
}
/*************************************W25QXX DRIVER END  *************************************/


