
#include "ALL_Includes.h"
#include "24l01.h"
#include "time.h"
#include "EXIT.h"
#include "remote.h"
#include "led.h"
#include "motor.h"
#include "ADC.h"
#include "eeprom.h"
#include"uartprotocol.h"
#include"uart.h"






#define  SYS_CLOCK    16


void CLOCK_Config(u8 SYS_CLK);
void All_Congfig(void);


int main(void)
{     
  int adcV[2];
  All_Congfig();
  UART_Init(16,115200,OnRecvData);

  

  delay_ms(100);


  while(1)
  {
    UART_SendChar((unsigned char)65);
    ADC_Init(Channel_3);
    adcV[0] = ADC_Data();
    delay_ms(50);
    ADC_Init(Channel_4);
    adcV[1] = ADC_Data();
    delay_ms(200);
  }
  

}
void All_Congfig(void)
{
    CLOCK_Config(SYS_CLOCK);//系统时钟初始化  
	
}

/// <summary>
///函数功能：系统内部时钟配置
///输入参数：SYS_CLK : 2、4、8、16
///输出参数：无
///备    注：系统启动默认内部2ＭＨＺ
/// </summary>
void CLOCK_Config(u8 SYS_CLK)
{
   //时钟配置为内部RC，16M
   CLK->CKDIVR &=~(BIT(4)|BIT(3));
  
   switch(SYS_CLK)
   {
      case 2: CLK->CKDIVR |=((1<<4)|(1<<3)); break;
      case 4: CLK->CKDIVR |=(1<<4); break;
      case 8: CLK->CKDIVR |=(1<<3); break;
   }
}



