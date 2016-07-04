#ifndef _ADC_H
#define _ADC_H

#include "iostm8s103f3.h"
#include "mcuhead.h"

typedef struct
{
  void (*Init)(void);
  u16 (*GetValue)(u8 CH);
}AdcBase;
extern const AdcBase ADC;


#endif
