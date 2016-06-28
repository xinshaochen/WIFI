#include "led.h"

void LED_Init()
{
	LED_R->DDR |= LED_R_P;
	LED_R->CR1 |= LED_R_P;
	LED_G->DDR |= LED_G_P;
	LED_G->CR1 |= LED_G_P;
	LED_B->DDR |= LED_B_P;
	LED_B->CR1 |= LED_B_P;

}



