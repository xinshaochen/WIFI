

#ifndef _ADC_H
#define _ADC_H

#include "stm8s.h"

#define Channel_2       0x02
#define Channel_3       0x03
#define Channel_4       0x04
#define Channel_5       0x05
#define Channel_6       0x06


/***************************
AD IO¿Úºê¶¨Òå
***************************/
#define ADC_GPIO        GPIOC
#define ADC_PIN         (1<<4)

#define AD0        GPIOC
#define AD0_P      (1<<4)
#define AD1        GPIOD
#define AD1_P      (1<<2)
#define AD2        GPIOD
#define AD2_P      (1<<3)
#define AD3        GPIOD
#define AD3_P      (1<<5)
#define AD4        GPIOD
#define AD4_P      (1<<6)



void ADC_Init(u8 Channel);
u16 ADC_Data(void);
void ADC_IOConfig(void);


#endif
