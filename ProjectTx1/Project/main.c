#include "iostm8s103f3.h"
#include "mcuhead.h"
#include "delay.h"
#include "led.h"
#include "ADC.h"
#include "uart.h"
#include "uartprotocol.h"
#include "string.h"
u8 ip[4]={0};

void ConnectSendCode(u8 *ip,u16 port,u8 cmd,u8 *buff,u16 len)
{
	u8 p[150];
	u8 p1[10];
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


void SysInit(u8 SysClk)
{
  CLK_CKDIVR &=~(BIT4|BIT3);
  switch(SysClk)
  {
    case 2:CLK_CKDIVR|=BIT4|BIT3; break;
    case 4:CLK_CKDIVR|=BIT4;break;
    case 8:CLK_CKDIVR|=BIT3;break;
  }
}

u8 reip[4]={0};
void AliveEvent(UartEvent e)
{
  u8 type[4];
  reip[0]=192;
  reip[1]=168;
  reip[2]=1;
  e->ReadBuff((char *)reip[3],1);
  
  
  
  ConnectSendCode("192.168.1.255",2333,0,&ip[3],sizeof(ip[3]));
  
  
  
//  if(strstr((const char*)type,"Rx0")!=NULL)
//  {
//    e->WriteString("useIP()");//
//    e->WriteString((u8 *)"Tx0");
//    e->SendAckPacket();
//  }
  
}
typedef enum GetDev
{
  AD=0,
}GetDev;
typedef enum SetDev
{
  S0=0,
}SetDev;
u16 CH3Vla,CH4Vla;
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
  case AD:
     e->WriteWord(CH3Vla);
     e->WriteWord(CH4Vla);
    break;
  }
  e->SendAckPacket();
}
u8 S0Data;
u16 lastSetId;
void SetDataEvent(UartEvent e)
{
  u8 id;
  u8 dev;
  id=e->ReadByte();
  dev=e->ReadByte();
  e->WriteByte(id);
  e->WriteByte(dev);
  e->SendAckPacket();//手动回应
  if(lastSetId==id) return ;
  lastSetId=id;
  switch((SetDev)dev)
  {
  case S0:
    S0Data=e->ReadByte();
    break;
  }
  
}
u8 getid=0;
u8 id[12]={0};

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
                        bufp[i]='\0';
			bufipp++;
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
		
		
	}
	
	
}

void ConnectSend(u8 *ip,u16 port,u8 *dat)
{
	u8 p[100];
	sprintf(p,"ConnectSend(\"%s\",%d,\"%s\")",ip,port,dat);
	UART_SendString(p);
}


void GetIPIDCmd()
{
	getid=1;
	UART.SendString("print(\"id:\"..node.chipid()..\" ip:\"..wifi.sta.getip()..\" \")");
}

//===================================================================
u16 sysCount=0;
u16 sysCount1=0;
u8 adcSele=3;
int main( void )
{
  SysInit(16);
  ADC.Init();
  UART.Init(16,115200,OnRecvData);
  UartProtocol.Init(databuff);
  UartProtocol.AutoAck(ENABLE);
  UartProtocol.RegisterCmd(Alive,AliveEvent);
  UartProtocol.RegisterCmd(GetData,GetDataEvent);
  UartProtocol.RegisterCmd(SetData,SetDataEvent);
  UartProtocol.RegisterCmd(DeErr,DeErrEvent);
  
  LED.Init();
  
  delay_ms(1000);
  GetIPIDCmd();
  while(1)
  {
    UartProtocol.Check();
    if(sysCount>=30)
    {
      sysCount=0;
      PD4_out=~PD4_out;
    }
    if(sysCount1>=2)
    {
      sysCount1=0;
      if(adcSele==3)
      {
        CH3Vla = ADC.GetValue(adcSele);
        adcSele=4;
      }else if(adcSele==4)
      {
        CH4Vla = ADC.GetValue(adcSele);
        adcSele=3;
      }
        
      
      
    }
    delay_ms(10);
    sysCount++;
    sysCount1++; 
  }
}


