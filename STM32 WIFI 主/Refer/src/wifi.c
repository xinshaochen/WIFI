#include "wifi.h"
#include <stm32f10x.h>
#include "Timer.h"
#include "string.h"
#include "lcd.h"
#include "stdio.h"


u8 recvID=0;//发送的确认ID
u8 retRecvID=0;//回来的确认ID
u8 recvarrayid[maxGdevEntry];
u8 *recvarraydata[maxGdevEntry];
u16 recvdatalen[maxGdevEntry]={0};
u8 recvdatabit[maxGdevEntry];//数据发送标志位

u8 sendID=0;//同上
u8 retSendID=0;
u8 sendarrayid[maxSdevEntry];
u8 *sendarraydata[maxGdevEntry];
u16 senddatalen[maxGdevEntry]={0};
u8 senddatabit[maxGdevEntry];//数据发送标志位


u8 remoteDevCount=0;///设备数量
s8 SeleRemoteDev=-1;//选择设备
DevData remoteDev[maxRemoteDev];//远端设备信息
DevData meDev;//本机

u8 sendIP[4];////发送的IP
u8 sendIPstr[16];

u8 scannCount=0;//扫描次数



static void Init(void)
{
	u8 i=0;
	for(i=0;i<maxRemoteDev;i++)
	{
	}
			

}
//开始扫描
/// 扫描标志位
bool scannBut=false;
static void ScannStart()
{
	remoteDevCount=0;
	SeleRemoteDev=-1;
		scannBut = true;
}
//客户端发送数据，带协议
static void ConnectSendCode(u8 *ip,u16 port,u8 cmd,u8 *buff,u16 len)
{
	u8 p[300];
	u8 p1[100];
	u8 buf;
	u8 sum=0;
	//sprintf(p,"ConnectSend(\"%s\",%d,{0xaa,0x%02X,0x%02X,0x%02X,",ip,port,_16T8H(len),_16T8L(len),cmd);
	sprintf(p,"ConnectSend(\"%s\",%d,{0xaa,%d,%d,%d,",ip,port,_16T8H(len),_16T8L(len),cmd);
	while(len--)
	{
		buf=*buff;
		//sprintf(p1,"0x%02x,",buf);
		sprintf(p1,"%d,",buf);
		strcat(p,p1);
		sum+=buf;
		buff++;
	}
	//sprintf(p1,"0x%02x,",sum);
	sprintf(p1,"%d,",sum);
	strcat(p,p1);
	//sprintf(p,"%s,%X",p1,sum);
	//sprintf(p1,"%s,0x55",p);
	strcat(p,"0x55})\r\n\0");
	UART_SendString(p);
}
//客户端发送数据，不带协议
static void ConnectSend(u8 *ip,u16 port,u8 *dat)
{
	u8 p[100];
	sprintf(p,"ConnectSend(\"%s\",%d,\"%s\")\r\n\0",ip,port,dat);
	UART_SendString(p);
}

///发送数据命令，带确认
static void setData(u8 sdev,u8 *dat,u16 len)
{
	u16 i=0;
	u16 p=0;
	if(SeleRemoteDev==-1) return ;
	if(remoteDevCount==0) return ;

	if(senddatalen[sdev]!=(len+2))
	{
		free(sendarraydata[sdev]);
		sendarraydata[sdev]=NULL;
		sendarraydata[sdev]=(u8 *)malloc((len+2)*sizeof(u8));
	}
	
	//if(sendarraydata[sdev]==NULL)
		
	//if(sendarrayid[sdev]==0)//屏蔽掉保持数据最新
	//{
	
	sendID++;
	
	*sendarraydata[sdev]=sendID;sendarraydata[sdev]++;p++;
	*sendarraydata[sdev]=sdev;sendarraydata[sdev]++;p++;
	for(i=0;i<len;i++)
	{
		*(sendarraydata[sdev]+i)=*(dat+i);
		p++;
	}
	sendarraydata[sdev]-=(p-1);
	
	senddatalen[sdev]=len+2;
	sendarrayid[sdev]=sendID;
	senddatabit[sdev]=1;
	//}
}
///发送获取数据命令，带确认
static void getData(u8 gdev)
{
	u8 i=0;
	u8 buff[2]={0};
	if(SeleRemoteDev==-1) return ;
	if(remoteDevCount==0) return ;

	if(recvdatalen[gdev]!=2)
	{
		free(recvarraydata[gdev]);
		recvarraydata[gdev]=NULL;
		recvarraydata[gdev]=(u8 *)malloc(2*sizeof(u8));//记得释放内存
	}
	
	//if(recvarraydata[gdev]==NULL)
		

	
	if(recvdatabit[gdev]==0)
	{
		recvID++;
	
		buff[0] = recvID;
		buff[1]=(u8)gdev;

		for(i=0;i<2;i++)
		{
			*(recvarraydata[gdev]+i) = buff[i];
		}
		recvdatalen[gdev]=2;
		recvarrayid[gdev] = recvID;
		recvdatabit[gdev]=1;//发送标志位
	}
}



//===========================================================================================
u8 getid=0;
//向WIFI模块获取IPID命令
static void GetIPIDCmd()
{
	getid=1;
	UART.SendString("print(\"id:\"..node.chipid()..\" ip:\"..wifi.sta.getip()..\" \")\r\n");
}
//扫描命令
static void ScannCmd()
{
	ConnectSendCode("192.168.1.255", 2333, 4, &meDev.IP[3], sizeof(meDev.IP[3]));
}
//===========================================================================================


//============================================================================================================
//============================================================================================================
//============================================================================================================
//UART回调服务
u8 getIPIDid;
//WIFI模块数据，非协议
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
		
		if(meDev.IP[0]!=0&&meDev.IP[1]!=0&&meDev.IP[2]!=0&&meDev.IP[3]!=0)
		Timer.Stop(getIPIDid);
		
		LCD_Fill(30,20,200,60,WHITE);
		LCD_ShowString(30,20,200,16,16,meDev.ID);	
		LCD_ShowString(30,40,200,16,16,bufip);	
	}
	
	
}
//========================================
//扫描数据
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
		if(i==remoteDevCount)//列表没有相同IP
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
//		e->WriteString((u8 *)"Rx0");//设备类型
//		e->SendAckPacket();
//	
//	}
//	
}

///接收IP，发送自己的IP
void SendMeIPEvent(UartEvent e)
{
	sendIP[0]=192;
	sendIP[1]=168;
	sendIP[2]=1;
	sendIP[3]=e->ReadByte();
	
	sprintf((char *)sendIPstr,"%d.%d.%d.%d\0",sendIP[0],sendIP[1],sendIP[2],sendIP[3]);
	ConnectSendCode(sendIPstr,2333,0,&meDev.IP[3],1);
	e->noack();//不要自动应答
}
//============================================================================================================
//============================================================================================================
//============================================================================================================

//============================================================================================================
//============================================================================================================
//============================================================================================================
//TIMER 服务  任务项
u8 countScann=0;
///发送服务
void sendServing()
{
	u8 i = 0;
	countScann++;
	if(countScann >= 6)//300ms一次
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
			if(scannCount>=8)//扫描次数
			{
				scannCount = 0;
				SeleRemoteDev=0;//选择第一个
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
		if(recvdatabit[i]==1)
		{
			ConnectSendCode(remoteDev[SeleRemoteDev].IPstr,2333,GetData,recvarraydata[i],recvdatalen[i]);
		}
	}
	for(i=0;i<maxSdevEntry;i++)
	{
		if(senddatabit[i]==1)
		{
			ConnectSendCode(remoteDev[SeleRemoteDev].IPstr,2333,SetData,sendarraydata[i],senddatalen[i]);
		}
	}
}
//拿IPID服务
void getIPIDServing()
{
	GetIPIDCmd();
	
}

//============================================================================================================
//============================================================================================================
//============================================================================================================
const wifiBase wifi={
	Init,
	ScannStart,
	ConnectSendCode,
	ConnectSend,
	setData,
	getData,
	{
		ScannCmd,
		GetIPIDCmd,
	},
	{
		DeErrEvent,
		AliveEvent,
		SendMeIPEvent,
	},
	{
		sendServing,
		getIPIDServing,
	},
	
};



