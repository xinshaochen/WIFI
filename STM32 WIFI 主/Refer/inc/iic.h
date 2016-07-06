#ifndef _IIC_H_
#define _IIC_H_
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"


//IO��������
#define SDA_IN()  {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=8<<20;}
#define SDA_OUT() {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=3<<20;}

//IO��������	 
#define IIC_SCL    PBout(12) //SCL
#define IIC_SDA    PBout(13) //SDA	 
#define READ_SDA   PBin(13)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  



#endif
