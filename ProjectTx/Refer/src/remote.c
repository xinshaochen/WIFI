#include "remote.h"
#include "ALL_Includes.h"


void Remote_Init(void) 
{

	sim();
	EXTI->CR1 |= BIT(6);
	EXTI->CR1 &= ~BIT(7);//CR1  上升沿触发
	//EXTI->CR1 |= BIT(7);
	//EXTI->CR1 &= ~BIT(6);//CR1  下升沿触发
	REMOTE->DDR &= ~REMOTE_P;
	REMOTE->CR1 |= REMOTE_P;
	REMOTE->CR2 |= REMOTE_P;




	TIM2->IER = 0x00;//禁止中断
	TIM2->EGR = 0x01;//允许产生更新标志
	TIM2->PSCR = 0x04;//设置时钟分频 2M/2=1MHz---1us
	TIM2->ARRH = 0x26;
	TIM2->ARRL = 0xA0;

	TIM2->CR1 |= 0x81;  //开启定时器
	TIM2->IER |= 0x01;  //允许中断 

	rim();       //rim使能中断

}
//遥控器接收状态
//[7]:收到了引导码标志
//[6]:得到了一个按键的所有信息
//[5]:保留	
//[4]:标记上升沿是否已经被捕获								   
//[3:0]:溢出计时器
u8 	RmtSta = 0;
u16 Dval;		//下降沿时计数器的值
u32 RmtRec = 0;	//红外接收到的数据	   		    
u8  RmtCnt = 0;	//按键按下的次数	 
//定时器2溢出中断
#pragma vector=TIM2_Updata_vector
__interrupt void TIM2_Updata_IRQHandler(void)
{
	sim();//关总中断
	if (RmtSta & 0x80)//上次有数据被接收到了
	{
		RmtSta &= ~0X10;						//取消上升沿已经被捕获标记
		if ((RmtSta & 0X0F) == 0X00)RmtSta |= 1 << 6;//标记已经完成一次按键的键值信息采集
		if ((RmtSta & 0X0F)<14)RmtSta++;
		else
		{
			RmtSta &= ~(1 << 7);//清空引导标识
			RmtSta &= 0XF0;	//清空计数器	
		}
	}
	TIM2->SR1 &= ~0x01;  // 清除更新中断标记，这步不能漏掉，否则会连续进入中断程序
	rim();//开总中断

}

#pragma   vector = EXTI3_PD_vector
__interrupt  void  EXTI3_PD_IRQHandler(void)
{
	if (REMOTE_S)
	{
		TIM2->CNTRL = 0;
		TIM2->CNTRH = 0;//清空定时器值
						EXTI->CR1 |= BIT(7);
						EXTI->CR1 &= ~BIT(6);//CR1  下升沿触发
		RmtSta |= 0X10;					//标记上升沿已经被捕获
	}
	else
	{
		Dval = ((u16)TIM2->CNTRH << 8) | TIM2->CNTRL;
		EXTI->CR1 |= BIT(6);
		EXTI->CR1 &= ~BIT(7);//CR1  上升沿触发
		if (RmtSta & 0X10)					//完成一次高电平捕获 
		{
			if (RmtSta & 0X80)//接收到了引导码
			{

				if (Dval>300 && Dval<800)			//560为标准值,560us
				{
					RmtRec <<= 1;	//左移一位.
					RmtRec |= 0;	//接收到0	   
				}
				else if (Dval>1400 && Dval<1800)	//1680为标准值,1680us
				{
					RmtRec <<= 1;	//左移一位.
					RmtRec |= 1;	//接收到1
				}
				else if (Dval>2200 && Dval<2600)	//得到按键键值增加的信息 2500为标准值2.5ms
				{
					RmtCnt++; 		//按键次数增加1次
					RmtSta &= 0XF0;	//清空计时器		
				}
			}
			else if (Dval>4200 && Dval<4700)		//4500为标准值4.5ms
			{
				RmtSta |= 1 << 7;	//标记成功接收到了引导码
				RmtCnt = 0;		//清除按键次数计数器
			}
		}
		RmtSta &= ~(1 << 4);
	}
}

//处理红外键盘
//返回值:
//	 0,没有任何按键按下
//其他,按下的按键键值.
u8 Remote_Scan(void)
{
	u8 sta = 0;
	u8 t1, t2;
	if (RmtSta&(1 << 6))//得到一个按键的所有信息了
	{
		t1 = RmtRec >> 24;			//得到地址码
		t2 = (RmtRec >> 16) & 0xff;	//得到地址反码 
		if ((t1 == (u8)~t2) && t1 == REMOTE_ID)//检验遥控识别码(ID)及地址 
		{
			t1 = RmtRec >> 8;
			t2 = RmtRec;
			if (t1 == (u8)~t2)sta = t1;//键值正确	 
		}
		if ((sta == 0) || ((RmtSta & 0X80) == 0))//按键数据错误/遥控已经没有按下了
		{
			RmtSta &= ~(1 << 6);//清除接收到有效按键标识
			RmtCnt = 0;		//清除按键次数计数器
		}
	}
	return sta;
}



