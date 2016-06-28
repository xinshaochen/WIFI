

#ifndef  _USART_H
#define  _USART_H

#include "ALL_Includes.h"
#include "EXIT.h"

//定义UART的TX、RX引脚
#define  TXPort  GPIOD
#define  TXPin   (1 << 5) 
#define  RXPort  GPIOD
#define  RXPin   (1 << 6)

void UART_Init(u8 SYS_Clk, u32 baud);
void UART_SendChar(unsigned char ch);
void UART_IOConfig(void);
void UART_Clost(void);

#endif