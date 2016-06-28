

#include "ADC.h"
#include "ALL_Includes.h"


/*********************************************
函数功能：ADC单通道转换初始化
输入参数：Channel：Channel_0——Channel_15
输出参数：AD数值
备    注：使用该函数前，需修改在ADC.h中的IO口宏定义
*********************************************/
void ADC_Init(u8 Channel)
{  
    ADC_IOConfig();
    
    ADC1->CSR &=0X00;
    ADC1->CSR |= Channel;//选择通道
    ADC1->CR1 &=~BIT(1);//单次转换模式
    
    ADC1->CR2 |= BIT(3);//数据右对齐
    
    ADC1->CR1 |= BIT(0);//把AD唤醒
}

/*********************************************
函数功能：获取ADC转换数据
输入参数：无
输出参数：AD数值
备    注：无
*********************************************/
u16 ADC_Data(void)
{
    u16 data=0;
    
    ADC1->CR1 |= BIT(0);//ADC使能
    
    while((ADC1->CSR&0X80)==0);//等待转换结束
    ADC1->CSR &=~BIT(7);//清除转换结束标志位

    data =  ADC1->DRL;
    data |= ((ADC1->DRH << 8) & 0x0300);
 
    return data;
}

/*********************************************
函数功能：ADC IO初始化 
输入参数：无
输出参数：无
备    注：通过修改宏定义改变所要初始化的IO
*********************************************/
void ADC_IOConfig(void)
{
    AD0->DDR &=~AD0_P;                  //输入模式 
    AD0->CR1 &=~AD0_P;                  //浮空输入模式    
    AD1->DDR &=~AD1_P;
    AD1->CR1 &=~AD1_P;
    AD2->DDR &=~AD2_P;
    AD2->CR1 &=~AD2_P;
    
    AD3->DDR &=~AD3_P;
    AD3->CR1 &=~AD3_P;
    AD4->DDR &=~AD4_P;
    AD4->CR1 &=~AD4_P;
    
}


