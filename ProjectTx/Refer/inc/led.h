#ifndef _LED_H_
#define _LED_H_
#include "stm8s.h"

#define LED_R GPIOA
#define LED_R_P (1<<1)
#define LED_G GPIOD
#define LED_G_P (1<<6)
#define LED_B GPIOD
#define LED_B_P (1<<5)

#define LED_R_SET LED_R->ODR|=LED_R_P
#define LED_R_CLR LED_R->ODR&=~LED_R_P
#define LED_G_SET LED_G->ODR|=LED_G_P
#define LED_G_CLR LED_G->ODR&=~LED_G_P
#define LED_B_SET LED_B->ODR|=LED_B_P
#define LED_B_CLR LED_B->ODR&=~LED_B_P




#endif 
