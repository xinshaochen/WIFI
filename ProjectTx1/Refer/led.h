#ifndef _LED_H_
#define _LED_H_

#define PD4_out  PD_ODR_ODR4  


typedef struct
{
  void (*Init)(void);
}LedBase;
extern const LedBase LED;

#endif 
