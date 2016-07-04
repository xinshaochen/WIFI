#include"uart.H"

void (*OnRecv)(u8);
static u32 f;
void UART_Init(u8 f_mhz,u32 b,UARTCALLBACK fun){
	f=f_mhz*1000000;
	OnRecv=fun;
	
    UART1_CR1=0x00;
    UART1_CR2=0x00;
    UART1_CR3=0x00;
    UART1_BRR2 = 0x0B;
    UART1_BRR1 = 0x08;     //115200波特率
    UART1_CR2_TEN = 1;     //允许发送
    UART1_CR2_REN = 1;     //允许接收
    UART1_CR2_RIEN = 1;    //接收中断使能
    asm("rim");
	
    //UART_ReSetBps(b);
}

//void UART_ReSetBps(u32 b)
//{
//	u32 baud=(25*f)/(4*b);
//	u32 t=(baud/100)<<4;
//	u32 t2=baud-(100*(t>>4));
//	USART1->CR1&=~USART_CR1_UE;
//	USART1->BRR=t|((((t2*16)+50)/100)&0x0f);
//	USART1->CR1|=USART_CR1_UE;
//}

void UART_SendByte(u8 dat){
	while((UART1_SR & 0x80)==0x00);
    UART1_DR = dat;
}

void UART_SendString(u8* p){
	while(*p)
		UART_SendByte(*p++);
}

#pragma   vector = UART1_R_RXNE_vector
__interrupt void uart1rxd(void)
{
    u8 dat=UART1_DR;
    UART1_SR_RXNE = 0;
	OnRecv(dat);
}

void UART_OnRecv(void(*p)(u8)){
	OnRecv=p;
}

const UARTBase UART={
	UART_Init,
	UART_SendByte,
	UART_SendString,
	//UART_ReSetBps,
	UART_OnRecv,
};
