#include "time.h"
#include "stm8s.h"
#include "led.h"
#include "motor.h"

#include "ALL_Includes.h"

u8 RL, GL, BL;//可调范围0~128

//定时器2初始化fmaster/分频=2M/2=1M,1us计数一次，50us 中断一次
void TIM2_Init(void)
{
  sim();       //sim是禁止中断
  TIM2->IER  = 0x00;  //禁止中断
 
  TIM2->EGR  =0x01;   //允许产生更新标志
  TIM2->PSCR =0x04;   //设置时钟分频 2M/2=1MHz---1us
  TIM2->ARRH = 0x26;  //
  TIM2->ARRL = 0xA0;  //       10ms   修正过的。

  
// 
//  TIM2->CNTRH=0x00;   //初值
//  TIM2->CNTRL=0x00;
// 
  TIM2->CR1 |= 0x81;  //开启定时器
  //TIM2->IER &= ~0x01;  //拒绝中断 
  TIM2->IER |= 0x01;  //允许中断 
  //sim();       //sim是禁止中断
  //TIM2->CR1 &= ~0x80;  //关闭定时器
 rim();       //rim使能中断
}




//==================================
//定时器时钟TIM_CLK=CPU_CLK/PSCR=1MHZ
//溢出频率=TIM_CLK/(ARR+1)=10KZ
//==================================
void TIM4_Init(void)
{
	TIM4->PSCR = 0x04;
	TIM4->ARR = 99;
	TIM4->IER |= BIT(0);

	TIM4->CR1 |= BIT(0);

	rim();
}


u8 i=0;
u8 motor_pwm=0;
u8 LPWM=0;
u8 RPWM=0;
#pragma vector = TIM4_Updata_vector
__interrupt void TIM4_Updata_IRQHandler(void)
{
    sim();//关总中断
//i++;
//if (i <= RL) LED_R_CLR;
//else LED_R_SET;
//if(i<=GL) LED_G_CLR;
//else LED_G_SET;
//if (i <= BL) LED_B_CLR;
//else LED_B_SET;
//
//if (i > 128) i = 0;
//    
    
    
    motor_pwm++; 
    if(motor_pwm>16) motor_pwm=0;
    
    if(motor_pwm<=LPWM) LeftPWM_SET;
    else LeftPWM_CLR;
    if(motor_pwm<=RPWM) RightPWM_SET;
      else RightPWM_CLR;
    
    
    
    TIM4->SR1 &=~BIT(0);//清标志位
    rim();//开总中断
}





