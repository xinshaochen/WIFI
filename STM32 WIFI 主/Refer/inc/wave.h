#ifndef _WAVE_H_
#define _WAVE_H_
#include "sys.h"
#include "delay.h"
#include "stm32f10x.h"

//trig1  PB10
//trig2  PB11
//echo1  PB0
//echo2  PB1

#define Trig1 PBout(10)
#define Trig2 PBout(11)

extern u8  TIM3CH3_CAPTURE_STA;
extern u16	TIM3CH3_CAPTURE_VAL;
extern u8  TIM3CH4_CAPTURE_STA;
extern u16	TIM3CH4_CAPTURE_VAL;


void TIM5CapInit(u16 arr,u16 psc);

void WaveInit(void);

u32 GetWaveTime(u8 ch);
u32 Time2Length(u32 time);


#endif
