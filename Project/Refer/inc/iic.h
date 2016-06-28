#ifndef _IIC_H_
#define _IIC_H_
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"


//IO方向设置
#define SDA_IN()  {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=8<<20;}
#define SDA_OUT() {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=3<<20;}

//IO操作函数	 
#define IIC_SCL    PBout(12) //SCL
#define IIC_SDA    PBout(13) //SDA	 
#define READ_SDA   PBin(13)  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  



#endif
