#include "led.h"
#include "mcuhead.h"

static void Init()
{
  PD_DDR|=BIT4;
  PD_CR1|=BIT4;
  PD_CR2|=BIT4;
}

const LedBase LED={
  Init,
};





