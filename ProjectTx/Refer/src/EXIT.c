#include "EXIT.h"
#include "ALL_Includes.h"




void EXTI_Init(void)
{
    CFG->GCR |= BIT(0);
    EXIT_GPIO->DDR &=~EXIT_PIN;                                                 //输入模式
    EXIT_GPIO->CR1 |= EXIT_PIN;       
    
    //上拉输入
    EXIT_GPIO->CR2 |= EXIT_PIN;                                                 //使能外部中断
     
    //EXTI->CR1  |= 0xc0;
    
    asm("rim");    //开总中断
}

#pragma   vector = EXTI0_PA_vector
__interrupt  void  EXTI0_PA_IRQHandler(void)
{     
  
}
#pragma   vector = EXTI1_PB_vector
__interrupt  void  EXTI1_PB_IRQHandler(void)
{     
  
}
#pragma   vector = EXTI2_PC_vector
__interrupt  void  EXTI2_PC_IRQHandler(void)
{     
  
}
//#pragma   vector = EXTI3_PD_vector
//__interrupt  void  EXTI3_PD_IRQHandler(void)
//{     
//  
//}
#pragma   vector = EXTI4_PE_vector
__interrupt  void  EXTI4_PE_IRQHandler(void)
{     
  
}





