#include "EXIT.h"
#include "ALL_Includes.h"




void EXTI_Init(void)
{
    CFG->GCR |= BIT(0);
    EXIT_GPIO->DDR &=~EXIT_PIN;                                                 //����ģʽ
    EXIT_GPIO->CR1 |= EXIT_PIN;       
    
    //��������
    EXIT_GPIO->CR2 |= EXIT_PIN;                                                 //ʹ���ⲿ�ж�
     
    //EXTI->CR1  |= 0xc0;
    
    asm("rim");    //�����ж�
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





