#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "stm8s.h"

#define LeftCw GPIOD 
#define LeftCwP (1<<5)

#define LeftCCw GPIOD
#define LeftCCwP (1<<6)

#define RightCw GPIOA
#define RightCwP (1<<1)

#define RightCCw GPIOA
#define RightCCwP (1<<2)

#define LeftPWM GPIOD
#define LeftPWMP (1<<4)

#define RightPWM GPIOA
#define RightPWMP (1<<3)


#define LeftCw_SET LeftCw->ODR|=LeftCwP
#define LeftCw_CLR LeftCw->ODR&=~LeftCwP
#define LeftCCw_SET LeftCCw->ODR|=LeftCCwP
#define LeftCCw_CLR LeftCCw->ODR&=~LeftCCwP
#define RightCw_SET RightCw->ODR|=RightCwP
#define RightCw_CLR RightCw->ODR&=~RightCwP
#define RightCCw_SET RightCCw->ODR|=RightCCwP
#define RightCCw_CLR RightCCw->ODR&=~RightCCwP
#define LeftPWM_SET LeftPWM->ODR|=LeftPWMP
#define LeftPWM_CLR LeftPWM->ODR&=~LeftPWMP
#define RightPWM_SET RightPWM->ODR|=RightPWMP
#define RightPWM_CLR RightPWM->ODR&=~RightPWMP


void MotorInit(void);
void Left_Run(s8 dat);
void Right_Run(s8 dat);




#endif
