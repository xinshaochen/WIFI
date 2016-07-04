#include "ADC.h"


static void Init(void)
{
  ADC_CR1_ADON=1;
  ADC_CSR_CH=3;
  ADC_CR2_ALIGN=1;//”“∂‘∆Î
  ADC_CR1_SPSEL=2;
}

static u16 GetValue(u8 CH)
{
  u8 dataH,dataL;
  u16 value=0;
  ADC_CSR_CH=CH;
  ADC_CR1_ADON=1;
  while(ADC_CSR_EOC==0);
  dataH=ADC_DRH;
  dataL=ADC_DRL;
  ADC_CSR_EOC=0;
  value=(dataH<<8)|dataL;
  return value;
}




const AdcBase ADC={
Init,
GetValue
};
