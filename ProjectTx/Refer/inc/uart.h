#ifndef uart_H
#define uart_H

#include"mcuhead.h"

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned long

typedef void(*UARTCALLBACK)(u8);
void UART_ReSetBps(u32 b);

#define UARTENABLE

typedef struct{
	void (*Init)(u8 f_MHz,u32 Baud,UARTCALLBACK CallBack);
	void (*SendByte)(u8);
	void (*SendString)(u8*);
	void (*SetBaud)(u32);
	void (*RegisterUartEvent)(void(*)(u8));
}UARTBase;

extern const UARTBase UART;

#endif
