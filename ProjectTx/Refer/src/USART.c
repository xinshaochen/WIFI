

#include "USART.h"
#include "ALL_Includes.h"
//#include "iostm8s103f3.h"
#include "EXIT.h"


/**************************
�������ܣ���ʼ��UART
���������SYS_Clk:ϵͳʱ�ӣ�2,4,8,16��
         baud��   ������
�����������
��    ע����
***************************/
void UART_Init(u8 SYS_Clk, u32 baud)
{   
    u16 UART_Temp;
  
    UART_IOConfig();//UART IO���ų�ʼ�� 
  
    
    UART1->CR1 = 0;// b5 = 0,����UART  b2 = 0,��ֹУ��
    UART1->CR2 = 0;// ��ֹUART���ͺͽ���                                                  
    UART1->CR3 = 0;// b5,b4 = 00,1��ֹͣλ
                            
/************************************************** 
    ���ò����ʣ�����ע�����¼��㣺
    (1) ������дBRR2
    (2) BRR1��ŵ��Ƿ�Ƶϵ���ĵ�11λ����4λ��
    (3) BRR2��ŵ��Ƿ�Ƶϵ���ĵ�15λ����12λ���͵�3λ����0λ
    ������ڲ�����λ9600ʱ����Ƶϵ��=2000000/9600=208
    ��Ӧ��ʮ��������Ϊ00D0��BBR1=0D,BBR2=00
*************************************************/ 
    
//    UART_Temp = SYS_Clk*1000000/baud;
//    
//    UART1->BRR2 = (u8)((UART_Temp&0x000F)|((UART_Temp&0xF000)>>8));
//    UART1->BRR1 = (u8)((UART_Temp&0x0FF0)>>4);
    
    UART1->BRR1=0x01;
    UART1->BRR2=0x01;
                                    
    UART1->CR2 = 0x2C; // b3 = 1,������
                       // b2 = 1,�������
                       // b5 = 1,������������ж� 
}


void UART_Clost(void)
{
  while((UART1->SR & 0x80) == 0x00);
    UART1->CR2 = 0x00;
    TXPort->DDR &=~TXPin;
    TXPort->CR1 &=~TXPin;
    RXPort->DDR &=~RXPin;
    RXPort->CR1 &=~RXPin;  
}
/**************************************
�������ܣ���UART3����һ���ַ�
���������ch -- Ҫ���͵��ַ�
�����������
��    ע����
***************************************/
void UART_SendChar(unsigned char data)
{
     while((UART1->SR & 0x80) == 0x00);  // �����ͼĴ������գ���ȴ�
     UART1->DR = data;                     // ��Ҫ���͵��ַ��͵����ݼĴ���  
}

/***********************************
�������ܣ�UART IO�ڳ�ʼ��
�����������
�����������
��   ע���ɣ������ģʽ�£���ͨ���ãң��Ĵ���
         �����������
***********************************/
void UART_IOConfig(void)
{ 
    TXPort->DDR |= TXPin;//���ģʽ
    TXPort->CR1 |= TXPin;//�������   
    
    RXPort->DDR &=~RXPin;//����ģʽ
    RXPort->CR1 &=~RXPin;//��������
}


#pragma vector = UART1_RX_vector
__interrupt void UART1_RX_IRQHandler(void)
{
  UART1->SR&=~BIT(5);
}






