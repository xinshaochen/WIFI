#ifndef  _EXIT_H
#define  _EXIT_H

#include "stm8s.h"

#define EXIT_GPIO         GPIOD
#define EXIT_PIN          (1<<4)

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


#define Infrared_Delay 2



void EXTI_Init(void);



#endif