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




//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart1_send_char(u8 c)
{
	while((USART1->SR&0X40)==0);//�ȴ���һ�η������   
	USART1->DR=c;   	
} 
//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//��������
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
}
//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
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
	usart1_niming_report(0XA1,tbuf,12);//�Զ���֡,0XA1
}	
//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//��0
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
	usart1_niming_report(0XAF,tbuf,28);//�ɿ���ʾ֡,0XAF
} 
//==================================================================================================
//==================================================================================================
//==================================================================================================
typedef enum GetDev
{
	ADC=0,
	Key,
	All,
}GetDev;
const u8 maxGdevEntry=3;
/////////////////////////
typedef enum SetDev
{
	PID=0,
	P2,
	P3,
	P4,
}SetDev;
const u8 maxSdevEntry=3;
/////////////////////////
u8 recvID=0;//���͵�ȷ��ID
u8 retRecvID=0;//������ȷ��ID
u8 recvarrayid[maxGdevEntry];
u8 *recvarraydata[maxGdevEntry] ;

u8 sendID=0;//ͬ��
u8 retSendID=0;
u8 sendarrayid[maxSdevEntry];
u8 *sendarraydata[maxGdevEntry];

typedef enum
{
	false=0,
	true,
}bool;


u8 scannCount=0;
/// ɨ���־λ
bool scannBut=false;

typedef struct
{
	u8 IP[4];
	u8 ID[12];
	u8 IPstr[16];
	
}DevData;
u8 remoteDevCount=0;///�豸����
s8 SeleRemoteDev=-1;//
DevData remoteDev[15];//Զ��
DevData meDev;//����
u8 sendIP[4];////���͵�IP
u8 sendIPstr[16];









	u32 time1,cm1,time2,cm2;

u8 onesid;


void AliveEvent(UartEvent e)
{
	u8 i;
	u8 l;
	u8 s=0;
	u8 buf[4];
	
	buf[0]=192;
	buf[1]=168;
	buf[2]=1;
	buf[3]=e->ReadByte();
	
	if (remoteDevCount >= 15) return;

	if(remoteDevCount>0)
	{
		for(i=0;i<remoteDevCount;i++)
		{
			for(l=0;l<4;l++)
			{
				if(buf[l]== remoteDev[i].IP[l])
				{
					s++;
				}
			}
			if(s>=4)
			{
				break;
			}else
			s=0;
		}
		if(i==remoteDevCount)//�б�û����ͬIP
		{
			
			for(l=0;l<4;l++)
			{
				remoteDev[remoteDevCount].IP[l] =buf[l];
			}
			sprintf((char *)remoteDev[remoteDevCount].IPstr,"%d.%d.%d.%d\0",remoteDev[remoteDevCount].IP[0],remoteDev[remoteDevCount].IP[1],
			remoteDev[remoteDevCount].IP[2],remoteDev[remoteDevCount].IP[3]);
			
			remoteDevCount++;
		}
		
	}else
	{
		for(l=0;l<4;l++)
		{
			remoteDev[0].IP[l]=buf[l];
		}
		sprintf((char *)remoteDev[remoteDevCount].IPstr,"%d.%d.%d.%d\0",remoteDev[remoteDevCount].IP[0],remoteDev[remoteDevCount].IP[1],
			remoteDev[remoteDevCount].IP[2],remoteDev[remoteDevCount].IP[3]);
		remoteDevCount++;

	}
	
	
	
//	e->ReadBuff(type,4);
//	if(strstr((const char*)type,"Tx0")!=NULL)
//	{
//		e->WriteString("useIP()");
//		e->WriteString((u8 *)"Rx0");//�豸����
//		e->SendAckPacket();
//	
//	}
//	
}


u16 adCH1=0;
u16 adCH2=0;
void GetDataEvent(UartEvent e)
{
	u8 id;
	u8 dev;
	id=e->ReadByte();
	dev=e->ReadByte();
	if(recvarrayid[dev]==id)
	{
		recvarrayid[dev]=0;
		retRecvID=id;
		free(recvarraydata[dev]);
		recvarraydata[dev]=NULL;
		
		adCH1 = e->ReadByte();
		adCH1<<=8;
		adCH1 |= e->ReadByte();
		
		adCH2 = e->ReadByte();
		adCH2<<=8;
		adCH2 |= e->ReadByte();
		
		
		
	}
	
	e->noack();
//	e->WriteByte(id);
//	e->WriteByte(dev);
//	switch((GetDev)dev)
//	{
//		case ADC:
//			e->WriteWord(cm2);	
//			break;
//		case Key:
//			break;
//		case All:
//			break;
//	}

//	e->SendAckPacket();
	
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
	e->SendAckPacket();	//�ֶ���Ӧ
	
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
//WIFIģ�����ݣ���Э��
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
		memset(meDev.ID,0,12);
		memset(meDev.IP,0,4);
		e->ReadBuff(buf,packlen);
		
		buf1 = strstr((char *)buf,"id:");
		if(buf1==NULL) return ;
		buf1+=3;
		i=0;
		while((*buf1) != ' ')
		{
			meDev.ID[i++] = *buf1;
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
			meDev.IP[f]=atoi((char *)bufp);
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
		meDev.IP[3] = atoi((char *)bufp);
		
		Timer.Stop(onesid);
		
		LCD_Fill(30,20,200,60,WHITE);
		LCD_ShowString(30,20,200,16,16,meDev.ID);	
		LCD_ShowString(30,40,200,16,16,bufip);	
	}
	
	
}



//�ͻ��˷������ݣ�����Э��
void ConnectSend(u8 *ip,u16 port,u8 *dat)
{
	u8 p[100];
	sprintf(p,"ConnectSend(\"%s\",%d,\"%s\")",ip,port,dat);
	UART_SendString(p);
}
//�ͻ��˷������ݣ���Э��
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
	strcat(p,"0x55})\0");
	UART_SendString(p);
}


///����IP�������Լ���IP
void SendMeIPEvent(UartEvent e)
{
	sendIP[0]=192;
	sendIP[1]=168;
	sendIP[2]=1;
	sendIP[3]=e->ReadByte();
	
	sprintf((char *)sendIPstr,"%d.%d.%d.%d\0",sendIP[0],sendIP[1],sendIP[2],sendIP[3]);
	ConnectSendCode(sendIPstr,2333,0,&meDev.IP[3],1);
	e->noack();//��Ҫ�Զ�Ӧ��
}

//��WIFIģ���ȡIPID����
void GetIPIDCmd()
{
	getid=1;
	UART.SendString("print(\"id:\"..node.chipid()..\" ip:\"..wifi.sta.getip()..\" \")\r\n");
}
void ScannCmd()
{
	ConnectSendCode("192.168.1.255", 2333, 4, &meDev.IP[3], sizeof(meDev.IP[3]));
}


void keyTick()
{
	u8 key;
	key=KEY_Scan(0);
	if(key==3)
	{
		LED0=~LED0;
		LCD_Fill(80,100,280,160,WHITE);
		remoteDevCount=0;
		scannBut = true;
		
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
	LCD_ShowNum(100, 200, adCH1, 4, 16);
	LCD_ShowNum(100, 220, adCH2, 4, 16);
		
		
}


void ones()
{
	GetIPIDCmd();
}

u8 countScann=0;
///���ͷ���
void sendServing()
{
	u8 i = 0;
	countScann++;
	if(countScann >= 6)//300msһ��
	{
		countScann=0;
		if(scannBut==true)
		{
			if (scannCount == 0)
			{
				remoteDevCount = 0;
			}
			scannCount++;
			ScannCmd();
			if(scannCount>=8)//ɨ�����
			{
				scannCount = 0;
				SeleRemoteDev=0;//ѡ���һ��
				scannBut=false;
				
				SeleRemoteDev=0;
				for (i = 0; i < remoteDevCount; i++)
				{
					LCD_ShowNum(81, 100 + (i * 20), remoteDev[i].IP[0], 3, 16);
					LCD_ShowNum(123, 100 + (i * 20), remoteDev[i].IP[1], 3, 16);
					LCD_ShowNum(165, 100 + (i * 20), remoteDev[i].IP[2], 3, 16);
					LCD_ShowNum(217, 100 + (i * 20), remoteDev[i].IP[3], 3, 16);
				}
			}
		}
	}
//==============================================================================
	if(SeleRemoteDev==-1) return ;
	if(remoteDevCount==0) return ;
	
	for(i=0;i<maxGdevEntry;i++)
	{	
		if(recvarraydata[i]!=NULL)
		{
			ConnectSendCode(remoteDev[SeleRemoteDev].IPstr,2333,GetData,recvarraydata[i],2);
		}
	}
	for(i=0;i<maxSdevEntry;i++)
	{
		
	}
}


///���ͻ�ȡ���������ȷ��
void getData(GetDev gdev)
{
	u8 i=0;
	u8 buff[2]={0};
	if(SeleRemoteDev==-1) return ;
	if(remoteDevCount==0) return ;

	if(recvarraydata[gdev]==NULL)
		recvarraydata[gdev]=(u8 *)malloc(2*sizeof(u8));//�ǵ��ͷ��ڴ�

	
	if(recvarrayid[gdev]==0)//����
	{
		recvID++;

	
		buff[0] = recvID;
		buff[1]=(u8)gdev;

		for(i=0;i<2;i++)
		{
			*(recvarraydata[gdev]+i) = buff[i];
		}

		recvarrayid[gdev] = recvID;
		//recvarraydata[gdev] = buff;
	}

		
	
}


void getDataTick()
{
	getData(ADC);
}

int main(void)
{
	float pitch,roll,yaw; 		//ŷ����
	short temp;					//�¶�
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
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
	UartProtocol.RegisterCmd(sendMeIP,SendMeIPEvent);

	Timer.Start(5,UartProtocol.Check);
	Timer.Start(50,keyTick);
	Timer.Start(50,showRefresh);
	Timer.Start(80,sendServing);
	Timer.Start(50,getDataTick);
	onesid = Timer.Start(1500,ones);
	
	
	delay_ms(1500);
	
	
	
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
	
	LCD_ShowString(0,100,200,16,16,"RemoteIP:     .     .      .   ");
	//GetIPIDCmd();   
	while(1)
	{
		Timer.Run();
	}

		//==============
	//��������
	
	while(1)
	{
		time1 = GetWaveTime(0);
		cm1 = Time2Length(time1);
		time2 = GetWaveTime(1);
		cm2 = Time2Length(time2);
		LCD_ShowNum(100,40,cm1,6,16);
		LCD_ShowNum(100,60,cm2,6,16);
		//printf("ʱ��1��%d  us        ʱ��2��%d  us\r\n",time1,time2);
	//	printf("����1��%d cm      ����2��%d cm\r\n",cm1,cm2);
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
			temp=MPU_Get_Temperature();	//�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//���Զ���֡���ͼ��ٶȺ�������ԭʼ����
			usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));

			if((t%10)==0)
			{ 
				if(temp<0)
				{
					LCD_ShowChar(30+48,40,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,40,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,40,temp/100,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,40,temp%10,1,16);		//��ʾС������ 
				temp=pitch*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,60,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,60,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,60,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,60,temp%10,1,16);		//��ʾС������ 
				temp=roll*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,80,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,80,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,80,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,80,temp%10,1,16);		//��ʾС������ 
				temp=yaw*10;
				if(temp<0)
				{
					LCD_ShowChar(30+48,100,'-',16,0);		//��ʾ����
					temp=-temp;		//תΪ����
				}else LCD_ShowChar(30+48,100,' ',16,0);		//ȥ������ 
				LCD_ShowNum(30+48+8,100,temp/10,3,16);		//��ʾ��������	    
				LCD_ShowNum(30+48+40,100,temp%10,1,16);		//��ʾС������  
				
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
