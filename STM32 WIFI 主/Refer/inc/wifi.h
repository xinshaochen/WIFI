#ifndef __WIFI_H
#define __WIFI_H
#include <stm32f10x.h>
#include "mcuhead.h"
#include "uart.h"
#include "uartprotocol.h"


typedef enum GetDev
{
	ADC=0,
	Key,
	All,
}GetDev;
#define maxGdevEntry  3

/////////////////////////
typedef enum SetDev
{
	DIR=0,
	P2,
	P3,
	P4,
}SetDev;
#define maxSdevEntry  4
/////////////////////////
#define maxRemoteDev 16//远端设备数量




typedef struct
{
	u8 IP[4];
	u8 ID[12];
	u8 IPstr[16];
	
}DevData;



extern u8 recvID;//发送的确认ID
extern u8 retRecvID;//回来的确认ID
extern u8 recvarrayid[maxGdevEntry];
extern u8 *recvarraydata[maxGdevEntry];
extern u16 recvdatalen[maxGdevEntry];
extern u8 recvdatabit[maxGdevEntry];//数据发送标志位

extern u8 sendID;//同上
extern u8 retSendID;
extern u8 sendarrayid[maxSdevEntry];
extern u8 *sendarraydata[maxGdevEntry];
extern u16 senddatalen[maxGdevEntry];
extern u8 senddatabit[maxGdevEntry];//数据发送标志位

extern u8 remoteDevCount;///设备数量
extern s8 SeleRemoteDev;//选择设备
extern DevData remoteDev[maxRemoteDev];//远端设备信息
extern DevData meDev;//本机

extern u8 sendIP[4];////发送的IP
extern u8 sendIPstr[16];

extern u8 scannCount;//扫描次数

//用于解除获取IPID任务
extern u8 getIPIDid;










typedef struct
{
	void (*Init)(void);
	//开始扫描
	void (*ScannStart)(void);
	//客户端发送数据，带协议
	void (*ConnectSendCode)(u8 *ip,u16 port,u8 cmd,u8 *buff,u16 len);
	//客户端发送数据，不带协议
	void (*ConnectSend)(u8 *ip,u16 port,u8 *dat);
	///发送数据设置，带确认
	void (*setData)(u8 sdev,u8 *dat,u16 len);
	///发送获取数据命令，带确认
	void (*getData)(u8 gdev);
	struct
	{
		//扫描命令
		void (*ScannCmd)(void);
		//IPID命令
		void (*GetIPIDCmd)(void);
	}cmd;
	struct
	{
//串口回调服务
		void (*DeErrEvent)(UartEvent e);
		void (*AliveEvent)(UartEvent e);
		void (*SendMeIPEvent)(UartEvent e);
	}uart;
	struct
	{
//TIMER 服务   任务项
		void (*sendServing)();
		void (*getIPIDServing)();
	}Task;
	
}wifiBase;

extern const wifiBase wifi;




#endif
