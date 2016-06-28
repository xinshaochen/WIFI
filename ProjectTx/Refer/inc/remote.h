#ifndef _REMOTE_H_
#define _REMOTE_H_
#include "ALL_Includes.h"

#define REMOTE GPIOD
#define REMOTE_P (1<<4)
#define REMOTE_S ((REMOTE->IDR&REMOTE_P)>>4)



#define   TLI_vector               0x02
#define   EXTI0_PA_vector          0x05
#define   EXTI1_PB_vector          0x06
#define   EXTI2_PC_vector          0x07
#define   EXTI3_PD_vector          0x08
#define   EXTI4_PE_vector          0x09
#define   SPI_vector               0x0C
#define   TIM1_Updata_vector       0x0D
#define   TIM1_Capture_vector      0x0E
#define   TIM2_Updata_vector       0x0F
#define   TIM2_Capture_vector      0x10
#define   UART1_TX_vector          0x13
#define   UART1_RX_vector          0x14
#define   I2C_vector               0x15
#define   ADC1_vector              0x18
#define   TIM4_Updata_vector       0x19


//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0      		   

extern u8 RmtCnt;	//�������µĴ���

void Remote_Init(void);

u8 Remote_Scan(void);

#endif

