#include "delay.h"
#include "usart.h"
#include "led.h"
#include "wave.h"
#include "lcd.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "uart.h"
#include "uartprotocol.h"
#include "key.h"
#include "string.h"
#include "Timer.h"
#include "adc.h"
#include "mcuhead.h"
#include "wifi.h"


//==================================================================================================


u16 adc1,adc2;
void GetDataEvent(UartEvent e)
{
//	u8 id;
//	u8 dev;
//	id=e->ReadByte();
//	dev=e->ReadByte();
//	if(recvarrayid[dev]==id)
//	{
//		retRecvID=id;
//	
//		recvdatabit[dev]=0;
//		
//		if(dev==ADC)
//		{
//			adCH1 = e->ReadByte();
//			adCH1<<=8;
//			adCH1 |= e->ReadByte();
//			
//			adCH2 = e->ReadByte();
//			adCH2<<=8;
//			adCH2 |= e->ReadByte();
//		}
//	}
//	e->noack();
	
	u8 s[6];
	u8 id;
	u8 dev;
	id=e->ReadByte();
	dev=e->ReadByte();
	s[0]=id;
	s[1]=dev;
	
	switch((GetDev)dev)
	{
		case ADC:
		s[2]=_16T8H(adc1);
		s[3]=_16T8L(adc1);
		s[4]=_16T8H(adc2);
		s[5]=_16T8L(adc2);
			break;
		case Key:
			break;
		case All:
			break;
	}
	if(sendIPstr!=NULL)
	wifi.ConnectSendCode(sendIPstr,2333,1,s,6);
	e->noack();
}


u8 DirVal=0;
void SetDataEvent(UartEvent e)
{
//	u8 id;
//	u8 dev;
//	id=e->ReadByte();
//	dev=e->ReadByte();
//	if(sendarrayid[dev]==id)
//	{
//		retSendID=id;
//		senddatabit[dev]=0;
//	}
//	e->noack();
//================================================	
	
	u8 s[2];
	u8 id;
	u8 dev;
	id=e->ReadByte();
	dev=e->ReadByte();
	s[0]=id;
	s[1]=dev;
	switch(dev)
	{
		case DIR:
			DirVal=e->ReadByte();
			break;
	}
	
	
	wifi.ConnectSendCode(sendIPstr,2333,2,s,6);
	e->noack();
}

void keyTask()
{
	u8 key;
	key=KEY_Scan(0);
	if(key==3)
	{
		//LED0=~LED0;
		//LCD_Fill(80,100,280,160,WHITE);
		wifi.ScannStart();
		
	}else if(key==2)
	{
		LED1=~LED1;
	}else if(key==1)
	{
		wifi.ConnectSendCode("192.168.1.255",2333,1,"abc",sizeof("abc"));
	}
}



//==============================
//显示Task
//
//==============================
void showRefresh()
{


}
//=============================
//GetData Task
//
//=============================
void getDataTask()
{
	wifi.getData(ADC);
}

//=============================
//ADC Task
//
//=============================
#define UpLimit 100
#define LowLimit 3900
void adcTask()
{
	static u8 lastDIR=0;
	adc1 = Get_Adc(ADC_CH1);
	adc2 = Get_Adc(ADC_CH2);
	if(adc1>UpLimit&&adc1<LowLimit&&adc2>UpLimit&&adc2<LowLimit)
	{
		DirVal=0;
	}else if(adc1<=UpLimit&&adc2>UpLimit&&adc2<LowLimit)
	{
		DirVal=1;
	}else if(adc1<=UpLimit&&adc2<=UpLimit)
	{
		DirVal=2;
	}else if(adc1>UpLimit&&adc1<LowLimit&&adc2<=UpLimit)
	{
		DirVal=3;
	}else if(adc1>=LowLimit&&adc2<=UpLimit)
	{
		DirVal=4;
	}else if(adc1>=LowLimit&&adc2>UpLimit&&adc2<LowLimit)
	{
		DirVal=5;
	}else if(adc1>=LowLimit&&adc2>=LowLimit)
	{
		DirVal=6;
	}else if(adc1>UpLimit&&adc1<LowLimit&&adc2>=LowLimit)
	{
		DirVal=7;
	}else if(adc1<=UpLimit&&adc2>=LowLimit)
	{
		DirVal=8;
	}
	
	
	//LCD_ShowNum(0, 300, DirVal, 1, 16);
	if(lastDIR!=DirVal)
	{
		lastDIR=DirVal;
		wifi.ConnectSendCode(sendIPstr,2333,SetData,&DirVal,1);
		//wifi.setData(DIR,&DirVal,1);
	}
}

int main(void)
{
	float pitch,roll,yaw; 		//欧拉角
	short temp;					//温度
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	u16 t=0;
	u16 k=0;
	u8 key;
	
	u8 lcd_id[12];		
 	Stm32_Clock_Init(9);
	delay_init(72);
	Timer.Init(72);
//	KEY_Init();
//	LED_Init();
//	WaveInit();
//	LCD_Init();
	Adc_Init();
	delay_ms(1500);delay_ms(500);
	UART.Init(72,115200,OnRecvData);
	UART.SendByte(0);
	
	UartProtocol.Init(buffdata);
	UartProtocol.AutoAck(ENABLE);
	UartProtocol.RegisterCmd(Alive,wifi.uart.AliveEvent);
	UartProtocol.RegisterCmd(DeErr,wifi.uart.DeErrEvent);
	UartProtocol.RegisterCmd(sendMeIP,wifi.uart.SendMeIPEvent);


	UartProtocol.RegisterCmd(GetData,GetDataEvent);
	UartProtocol.RegisterCmd(SetData,SetDataEvent);


	Timer.Start(5,UartProtocol.Check);
	Timer.Start(100,wifi.Task.sendServing);
	getIPIDid = Timer.Start(1500,wifi.Task.getIPIDServing);
	
	Timer.Start(50,keyTask);
	//Timer.Start(50,showRefresh);
	Timer.Start(50,getDataTask);
	Timer.Start(80,adcTask);
	
	
	
	
	
	while(1)
	{
		Timer.Run();
	}

	
	MPU_Init();	
//	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);		 
	LCD_ShowString(30,20,200,16,16,"MPU6050 TEST ");
// 	LCD_ShowString(30,110,200,16,16,lcd_id);	  					
 	LCD_ShowString(30,40,200,16,16," Temp:    . C");	
 	LCD_ShowString(30,60,200,16,16,"Pitch:    . C");	
 	LCD_ShowString(30,80,200,16,16," Roll:    . C");	 
 	LCD_ShowString(30,100,200,16,16," Yaw :    . C");	 
	
	LCD_ShowString(30,120,200,16,16," Ax :    ");	 
	LCD_ShowString(30,140,200,16,16," Ay :    ");	 
	LCD_ShowString(30,160,200,16,16," Az :    ");	 
	LCD_ShowString(30,180,200,16,16," Gx :    ");	 
	LCD_ShowString(30,200,200,16,16," Gy :    ");	 
	LCD_ShowString(30,220,200,16,16," Gz :    ");	 
	
	while(mpu_dmp_init())
	{
		LCD_ShowString(30,20,200,16,16,"MPU6050 Error");
		delay_ms(200);
		LCD_Fill(30,20,239,20+16,WHITE);
 		delay_ms(200);
	}
	
	
	while(1)
	{
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			temp=MPU_Get_Temperature();	//得到温度值
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
			mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//用自定义帧发送加速度和陀螺仪原始数据
			usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));

			if((t%10)==0)
			{ 
				if(temp<0)
				{
					LCD_ShowChar(30+48,40,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else LCD_ShowChar(30+48,40,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,40,temp/100,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,40,temp%10,1,16);		//显示小数部分 
				temp=pitch*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,60,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else LCD_ShowChar(30+48,60,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,60,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,60,temp%10,1,16);		//显示小数部分 
				temp=roll*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,80,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else LCD_ShowChar(30+48,80,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,80,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,80,temp%10,1,16);		//显示小数部分 
				temp=yaw*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,100,'-',16,0);		//显示负号
					temp=-temp;		//转为正数
				}else LCD_ShowChar(30+48,100,' ',16,0);		//去掉负号 
				LCD_ShowNum(30+48+8,100,temp/10,3,16);		//显示整数部分	    
				LCD_ShowNum(30+48+40,100,temp%10,1,16);		//显示小数部分  
				
				temp=aacx;
				if(temp<0)
				{
					LCD_ShowChar(30+48,120,'-',16,0);	
					temp=-temp;
				}else LCD_ShowChar(30+48,120,' ',16,0);
				LCD_ShowNum(30+48+8,120,temp,3,16);   
				temp=aacy;
				if(temp<0)
				{
					LCD_ShowChar(30+48,140,'-',16,0);	
					temp=-temp;
				}else LCD_ShowChar(30+48,140,' ',16,0);
				LCD_ShowNum(30+48+8,140,temp,3,16);   
				temp=aacz;
				if(temp<0)
				{
					LCD_ShowChar(30+48,160,'-',16,0);	
					temp=-temp;
				}else LCD_ShowChar(30+48,160,' ',16,0);
				LCD_ShowNum(30+48+8,160,temp,3,16);   
				temp=gyrox;
				if(temp<0)
				{
					LCD_ShowChar(30+48,180,'-',16,0);	
					temp=-temp;
				}else LCD_ShowChar(30+48,180,' ',16,0);
				LCD_ShowNum(30+48+8,180,temp,3,16);   
				temp=gyroy;
				if(temp<0)
				{
					LCD_ShowChar(30+48,200,'-',16,0);	
					temp=-temp;
				}else LCD_ShowChar(30+48,200,' ',16,0);
				LCD_ShowNum(30+48+8,200,temp,3,16);   
				temp=gyroz;
				if(temp<0)
				{
					LCD_ShowChar(30+48,220,'-',16,0);	
					temp=-temp;
				}else LCD_ShowChar(30+48,220,' ',16,0);
				LCD_ShowNum(30+48+8,220,temp,3,16);   
				
				
				t=0;

			}
		}
		t++;
	}

	
	
}
