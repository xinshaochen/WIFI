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


u8 buffdata[RECV_BUFF_LEN+SEND_BUFF_LEN];




//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{
	while((USART1->SR&0X40)==0);//等待上一次发送完毕   
	USART1->DR=c;   	
} 
//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}
//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	usart1_niming_report(0XA1,tbuf,12);//自定义帧,0XA1
}	
//通过串口1上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//清0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	usart1_niming_report(0XAF,tbuf,28);//飞控显示帧,0XAF
} 

	u32 time1,cm1,time2,cm2;



u8 reip[4]={0};
void AliveEvent(UartEvent e)
{


	u8 type[4];
	
	reip[0]=192;
  reip[1]=168;
  reip[2]=1;
	reip[3] = e->ReadByte(1);
	
	LCD_ShowNum(81,100,reip[0],3,16);
	LCD_ShowNum(123,100,reip[1],3,16);
	LCD_ShowNum(165,100,reip[2],3,16);
	LCD_ShowNum(207,100,reip[3],3,16);
//	e->ReadBuff(type,4);
//	if(strstr((const char*)type,"Tx0")!=NULL)
//	{
//		e->WriteString("useIP()");
//		e->WriteString((u8 *)"Rx0");//设备类型
//		e->SendAckPacket();
//	
//	}
//	
}


typedef enum GetDev
{
	Wave=0,
	Key,
	All,
}GetDev;
typedef enum SetDev
{
	PID=0,
	P2,
	P3,
	P4,
}SetDev;

void GetDataEvent(UartEvent e)
{
	u8 id;
	u8 dev;
	id=e->ReadByte();
	dev=e->ReadByte();
	
	e->WriteByte(id);
	e->WriteByte(dev);
	switch((GetDev)dev)
	{
		case Wave:
			e->WriteWord(cm2);	
			break;
		case Key:
			break;
		case All:
			break;
	}

	e->SendAckPacket();
	
}


typedef struct
{
	s16 P;
	s16 I;
	s16 D;
}PIDstruct;
PIDstruct pid;
u16 lastSetId;
void SetDataEvent(UartEvent e)
{
	u8 id;
	u8 dev;
	id=e->ReadByte();
	dev = e->ReadByte();
	e->WriteByte(id);
	e->WriteByte(dev);
	e->SendAckPacket();	//手动回应
	
	if(lastSetId==id)return ;
	lastSetId=id;
	switch((SetDev)dev)
	{
		case PID:
			pid.P = e->ReadByte();
			pid.I = e->ReadByte();
			pid.D = e->ReadByte();
			break;
		case P2:
			break;
		case P3:
			break;
		case P4:
			break;
			
	}
	

	
}


u8 getid=0;
u8 id[12]={0};
u8 ip[4]={0};

void DeErrEvent(UartEvent e)
{
	u8 buf[50]={0};
u8 bufip[15]={0};
u8 *bufipp;
u8 bufp[4]={0};
	char *buf1;
	char *buf2;
u8 i;
u8 f;
	if(getid==1)
	{
		getid=0;
		memset(id,0,12);
		memset(ip,0,15);
		e->ReadBuff(buf,packlen);
		
		buf1 = strstr((char *)buf,"id:");
		if(buf1==NULL) return ;
		buf1+=3;
		i=0;
		while((*buf1) != ' ')
		{
			id[i++] = *buf1;
			buf1++;
		}
		
		buf2 = strstr((char *)buf1,"ip:");
		if(buf2==NULL) return;
		buf2+=3;
		i=0;
		while((*buf2) != ' ')
		{
			bufip[i++]=*buf2;
			buf2++;
		}
		
		bufipp=(u8 *)bufip;
		for(f=0;f<3;f++)
		{
			i=0;
			bufp[0]=bufp[1]=bufp[2]=0;
			while(*bufipp!='.')
			{
				bufp[i] = *bufipp;
				bufipp++;
				i++;
			}
			bufipp++;
			bufp[i]='\0';
			ip[f]=atoi((char *)bufp);
		}
		i=0;
		bufp[0]=bufp[1]=bufp[2]=0;
		while(*bufipp!='\0')
		{
			bufp[i] = *bufipp;
			bufipp++;
			i++;
		}
		bufp[i]='\0';
		ip[3] = atoi((char *)bufp);
		
		
		LCD_Fill(30,20,200,60,WHITE);
		LCD_ShowString(30,20,200,16,16,id);	
		LCD_ShowString(30,40,200,16,16,bufip);	
	}
	
	
}

void ConnectSend(u8 *ip,u16 port,u8 *dat)
{
	u8 p[100];
	sprintf(p,"ConnectSend(\"%s\",%d,\"%s\")",ip,port,dat);
	UART_SendString(p);
}

void ConnectSendCode(u8 *ip,u16 port,u8 cmd,u8 *buff,u16 len)
{
	u8 p[300];
	u8 p1[100];
	u8 buf;
	u8 sum=0;
	sprintf(p,"ConnectSend(\"%s\",%d,{0xaa,0x%02X,0x%02X,0x%02X,",ip,port,_16T8H(len),_16T8L(len),cmd);
	
	while(len--)
	{
		buf=*buff;
		sprintf(p1,"0x%02x,",buf);
		strcat(p,p1);
		sum+=buf;
		buff++;
	}
	sprintf(p1,"0x%02x,",sum);
	strcat(p,p1);
	//sprintf(p,"%s,%X",p1,sum);
	//sprintf(p1,"%s,0x55",p);
	strcat(p,"0x55})");
	UART_SendString(p);
}
void Broadcast(u16 port,u8 *dat)
{
	ConnectSend("192.168.1.255",port,dat);
}
void BroadcastCode(u16 port,u8 cmd,u8 *buff)
{
	ConnectSendCode("192.168.1.255",port,cmd,buff,sizeof(buff));
}
void GetIPIDCmd()
{
	getid=1;
	UART.SendString("print(\"id:\"..node.chipid()..\" ip:\"..wifi.sta.getip()..\" \")");
}
void ScanCmd()
{
	
}


void keyTick()
{
	u8 key;
	key=KEY_Scan(0);
	if(key==3)
	{
		LED0=~LED0;
		ConnectSendCode("192.168.1.255",2333,0,&ip[3],sizeof(ip[3]));
		
	}else if(key==2)
	{
		LED1=~LED1;
		GetIPIDCmd();
	}else if(key==1)
	{
		ConnectSendCode("192.168.1.255",2333,1,"abc",sizeof("abc"));
	}
}
void showRefresh()
{

if(reip[0]!=0)
{
	
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
	
	u8 buff[]="Rx0";
	
	u8 lcd_id[12];		
 	Stm32_Clock_Init(9);
	//uart_init(72,9600);
	//uart_init(72,500000);
	delay_init(72);
	Timer.Init(72);
	KEY_Init();
	LED_Init();
	WaveInit();
	LCD_Init();
	
	
	
	UART.Init(72,115200,OnRecvData);
	UART.SendByte(0);
	//UART.SendByte(0xaa);
	
	UartProtocol.Init(buffdata);
	UartProtocol.AutoAck(ENABLE);
	UartProtocol.RegisterCmd(Alive,AliveEvent);
	UartProtocol.RegisterCmd(GetData,GetDataEvent);
	UartProtocol.RegisterCmd(SetData,SetDataEvent);
	UartProtocol.RegisterCmd(DeErr,DeErrEvent);

	Timer.Start(5,UartProtocol.Check);
	Timer.Start(50,keyTick);
	Timer.Start(150,showRefresh);
	
	
	delay_ms(1000);
	
	
	
//	LCD_ShowString(30,60,200,16,16,"Wave2 Val:");
//	LCD_ShowString(30,60,200,16,16,"Wave2 Val:");
//	LCD_ShowString(30,100,200,16,16,"P:");
//	LCD_ShowString(30,120,200,16,16,"I:");
//	LCD_ShowString(30,140,200,16,16,"D:");
//	
	//getid=1;
	//UART.SendString("print(node.chipid())");
	LCD_ShowString(0,20,200,16,16,"ID:");
	LCD_ShowString(0,40,200,16,16,"IP:");
	
	LCD_ShowString(0,100,200,16,16,"RemoteIP:     .     .     .");
	GetIPIDCmd();
	while(1)
	{
		Timer.Run();
	}

		//==============
	//超声波部
	
	while(1)
	{
		time1 = GetWaveTime(0);
		cm1 = Time2Length(time1);
		time2 = GetWaveTime(1);
		cm2 = Time2Length(time2);
		LCD_ShowNum(100,40,cm1,6,16);
		LCD_ShowNum(100,60,cm2,6,16);
		//printf("时间1：%d  us        时间2：%d  us\r\n",time1,time2);
	//	printf("距离1：%d cm      距离2：%d cm\r\n",cm1,cm2);
		delay_ms(500);
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
