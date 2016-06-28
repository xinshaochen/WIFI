#include "motor.h"
#include "stm8s.h"
#include "time.h"
#include "stdlib.h"


void MotorInit(void)
{
  LeftCw->DDR|=LeftCwP;
  LeftCw->CR1|=LeftCwP;
  LeftCCw->DDR|=LeftCCwP;
  LeftCCw->CR1|=LeftCCwP;
  RightCw->DDR|=RightCwP;
  RightCw->CR1|=RightCwP;
  RightCCw->DDR|=RightCCwP;
  RightCCw->CR1|=RightCCwP;
  
  LeftPWM->DDR|=LeftPWMP;
  LeftPWM->CR1|=LeftPWMP;
  RightPWM->DDR|=RightPWMP;
  RightPWM->CR1|=RightPWMP;
  

}


void Left_Run(s8 dat)
{       
  if(dat>=0)
  {
    LeftCw_CLR;
    LeftCCw_SET;
    
  }
  else
  {
    LeftCw_SET;
    LeftCCw_CLR;
  }
  LPWM=abs(dat);
}
void Right_Run(s8 dat)
{
  if(dat>=0)
  {
    RightCw_CLR;
    RightCCw_SET;
  }
  else
  {
    RightCw_SET;
    RightCCw_CLR;
  }
  RPWM=abs(dat);
}
