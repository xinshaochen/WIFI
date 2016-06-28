#ifndef _24L01_H_
#define _24L01_H_
#include "stm8s.h"

#define   uint    u16
#define   uchar   u8


//========================================
//引脚定义
#define CE GPIOC
#define CE_P (1<<3)
#define CSN GPIOC
#define CSN_P (1<<4)
#define SCK GPIOC
#define SCK_P (1<<5)
#define MOSI GPIOC
#define MOSI_P (1<<6)
#define MISO GPIOC
#define MISO_P (1<<7)
#define IRQ GPIOD
#define IRQ_P (1<<3)

#define CE_SET CE->ODR|=CE_P
#define CE_CLR CE->ODR&=~CE_P
#define CSN_SET CSN->ODR|=CSN_P
#define CSN_CLR CSN->ODR&=~CSN_P
#define SCK_SET SCK->ODR|=SCK_P
#define SCK_CLR SCK->ODR&=~SCK_P
#define MOSI_SET MOSI->ODR|=MOSI_P
#define MOSI_CLR MOSI->ODR&=~MOSI_P

#define MISO_S ((MISO->IDR&MISO_P)>>7)
#define IRQ_S  ((IRQ->IDR&IRQ_P)>>3)




/*****************************************************************
nRF24L01指令格式
*******************************************************************/
#define R_REGISTER    0x00//读寄存器
#define W_REGISTER    0x20//写寄存器
#define R_RX_PLOAD    0x61//读RX FIFO有效数据，1-32字节，当读数据完成后，数据被清除，应用于接收模式 
#define W_TX_PLOAD    0xA0//写TX FIFO有效数据，1-32字节，写操作从字节0开始，应用于发射模式 
#define FLUSH_TX      0xE1//清除TX FIFO寄存器，应用于发射模式 
#define FLUSH_RX      0xE2//清除RX FIFO寄存器，应用于接收模式 
#define REUSE_TX_PL   0xE3//重新使用上一包有效数据，当CE为高过程中，数据包被不断的重新发射 
#define NOP           0xFF//空操作，可以用来读状态寄存器


/******************************************************************
nRF24L01寄存器地址
*******************************************************************/

#define CONFIG      0x00  // 配置寄存器 
#define EN_AA       0x01  // “自动应答”功能寄存 
#define EN_RX_ADDR  0x02  // 接收通道使能寄存器 
#define SETUP_AW    0x03  // 地址宽度设置寄存器 
#define SETUP_RETR  0x04  // 自动重发设置寄存器 
#define RF_CH       0x05  // 射频通道频率设置寄存器 
#define RF_SETUP    0x06  // 射频设置寄存器 
#define STATUS      0x07  // 状态寄存器 
#define OBSERVE_TX  0x08  // 发送检测寄存器

#define CD          0x09  // 载波检测寄存器 
#define RX_ADDR_P0  0x0A  // 数据通道0接收地址寄存器 
#define RX_ADDR_P1  0x0B  // 数据通道1接收地址寄存器 
#define RX_ADDR_P2  0x0C  // 数据通道2接收地址寄存器 
#define RX_ADDR_P3  0x0D  // 数据通道3接收地址寄存器 
#define RX_ADDR_P4  0x0E  // 数据通道4接收地址寄存器 
#define RX_ADDR_P5  0x0F  // 数据通道5接收地址寄存器 
#define TX_ADDR     0x10  // 发送地址寄存器 
#define RX_PW_P0    0x11  // 数据通道0有效数据宽度设置寄存器 
#define RX_PW_P1    0x12  // 数据通道1有效数据宽度设置寄存器 
#define RX_PW_P2    0x13  // 数据通道2有效数据宽度设置寄存器 
#define RX_PW_P3    0x14  // 数据通道3有效数据宽度设置寄存器 
#define RX_PW_P4    0x15  // 数据通道4有效数据宽度设置寄存器 
#define RX_PW_P5    0x16  // 数据通道5有效数据宽度设置寄存器 
#define FIFO_STATUS 0x17  // FIFO状态寄存器


#define MAX_TX  		0x10  //达到最大发送次数中断
#define TX_OK   		0x20  //TX发送完成中断
#define RX_OK   		0x40  //接收到数据中断




//24L01发送接收数据宽度定义
#define TX_ADR_WIDTH    5   	//5字节的地址宽度
#define RX_ADR_WIDTH    5   	//5字节的地址宽度
#define TX_PLOAD_WIDTH  32  	//32字节的用户数据宽度
#define RX_PLOAD_WIDTH  32  	//32字节的用户数据宽度


void NRF24L01_Init(void);
uchar SPI_RW(uchar byte);
uchar SPI_W_Reg(uchar reg,uchar value);
uchar SPI_R_byte(uchar reg);
uchar SPI_R_DBuffer(uchar reg,uchar *Dat_Buffer,uchar Dlen);
uchar SPI_W_DBuffer(uchar reg,uchar *TX_Dat_Buffer,uchar Dlen);

uchar NRF24L01_Check(void);

void NRF24L01_TX_Mode(void);
/// <summary>
/// 发送函数
/// </summary>
/// <param name="txbuf">字符串</param>
u8 NRF24L01_TxPacket(u8 *txbuf);

void NRF24L01_RX_Mode(void);

u8 NRF24L01_RxPacket(u8 *rxbuf);


#endif
