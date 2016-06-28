#include"uart.H"
#include "stm8s.h"

void (*OnRecv)(u8);
static u32 f;
void UART_Init(u8 f_mhz,u32 b,UARTCALLBACK fun){
	f=f_mhz*1000000;
	OnRecv=fun;
	
    UART1->CR1=0x00;
    UART1->CR2=0x00;
    UART1->CR3=0x00;
    UART1->BRR2 = 0x0B;
    UART1->BRR1 = 0x08;     //115200波特率
    UART1->CR2=0x2c;     //允许发送
	
    //UART_ReSetBps(b);
}

//void UART_ReSetBps(u32 b)
//{
//	u32 baud=(25*f)/(4*b);
//	u32 t=(baud/100)<<4;
//	u32 t2=baud-(100*(t>>4));
//	UART1->CR1&=~BIT5;
//        
//	UART1->BRR1=t|((((t2*16)+50)/100)&0x0f);
//	UART1->CR1|=BIT5;
//}

void UART_SendByte(u8 dat){
	while((UART1->SR & 0x80)==0x00);
    UART1->DR = dat;
}

void UART_SendString(u8* p){
	while(*p)
		UART_SendByte(*p++);
}

#pragma   vector = UART1_R_RXNE_vector
__interrupt void uart1rxd(void)
{
    u8 dat=UART1_DR;
    UART1->SR &= ~BIT5;
	OnRecv(dat);
}

void UART_OnRecv(void(*p)(u8)){
	OnRecv=p;
}

const UARTBase UART={
	UART_Init,
	UART_SendByte,
	UART_SendString,
	UART_ReSetBps,
	UART_OnRecv
};
