#include "24l01.h"
#include "ALL_Includes.h"
#include "delay.h"


const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址


uchar sta;    // 状态变量 


void NRF24L01_Init(void)
{
  CE->DDR|=CE_P;
  CE->CR1|=CE_P;
  CSN->DDR|=CSN_P;
  CSN->CR1|=CSN_P;
  SCK->DDR|=SCK_P;
  SCK->CR1|=SCK_P;
  MOSI->DDR|=MOSI_P;
  MOSI->CR1|=MOSI_P;
  
  MISO->DDR&=~MISO_P;
  MISO->CR1|=MISO_P;
  
  IRQ->DDR&=~IRQ_P;
  IRQ->CR1|=IRQ_P;
  
  CE_CLR;
  CSN_SET;
  SCK_CLR;
  
    
}

uchar SPI_RW(uchar byte)
{
  uchar i;
  for(i=0;i<8;i++)
  {
    if(byte&0x80)
      MOSI_SET;
    else 
      {MOSI_CLR;}
    byte<<=1;
    SCK_SET;
    if(MISO_S)
      {byte|=0x01;}
    SCK_CLR;
  }
  return byte;
}

//=========================================
//SPI写寄存器一个字节
//reg:寄存器地址
//value:值
uchar SPI_W_Reg(uchar reg,uchar value)
{
  uchar status;
  CSN_CLR;
  status=SPI_RW(reg);
  SPI_RW(value);
  CSN_SET;
  return status;
}

//==================================
//读一个寄存器
uchar SPI_R_byte(uchar reg)
{
  uchar reg_value;
  CSN_CLR;
  SPI_RW(reg);//地址
  reg_value=SPI_RW(0);//读取
  CSN_SET;
  return reg_value;
}

//===================================
//SPI读取RXFIFO 寄存器数据
//reg:寄存器地址
//Dat_Buffer:读取到的数据
//Dlen:数据长度
uchar SPI_R_DBuffer(uchar reg,uchar *Dat_Buffer,uchar Dlen)
{
  uchar status,i;
  CSN_CLR;
  status=SPI_RW(reg);//写寄存器地址
  for(i=0;i<Dlen;i++)
  {
    Dat_Buffer[i]=SPI_RW(0);//取数据
  }
  CSN_SET;
  return status;
}
//=================================
//SPI写TXFIFO 数据
//reg:写入寄存器地址
//TX_Dat_Buffer:要发生的数据
//Dlen:数据长度
uchar SPI_W_DBuffer(uchar reg,uchar *TX_Dat_Buffer,uchar Dlen)
{
  uchar status,i;
  CSN_CLR;
  status=SPI_RW(reg);
  for(i=0;i<Dlen;i++)
  {
    SPI_RW(TX_Dat_Buffer[i]);
  }
  CSN_SET;
  return status;
}


uchar NRF24L01_Check(void)
{
  u8 buff[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
  u8 buf[5]={0,0,0,0,0};
  u8 i;
  
  SPI_W_DBuffer(W_REGISTER+TX_ADDR,buff,5);
    SPI_R_DBuffer(TX_ADDR,buf,5);
    for(i=0;i<5;i++) if(buf[i]!=0XA5) break;
    if(i!=5) return 1;
    return 0;
      
}



//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
void NRF24L01_TX_Mode(void)
{														 
	CE_CLR;	    
  SPI_W_DBuffer(W_REGISTER+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
  SPI_W_DBuffer(W_REGISTER+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

  SPI_W_Reg(W_REGISTER+EN_AA,0x01);     //使能通道0的自动应答    
  SPI_W_Reg(W_REGISTER+EN_RX_ADDR,0x01); //使能通道0的接收地址  
  SPI_W_Reg(W_REGISTER+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  SPI_W_Reg(W_REGISTER+RF_CH,40);       //设置RF通道为40
  SPI_W_Reg(W_REGISTER+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  SPI_W_Reg(W_REGISTER+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	CE_SET;//CE为高,10us后启动发送
        
        delay_ms(1);
}

//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
 	
	CE_CLR;
        SPI_W_DBuffer(W_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  32个字节
 	CE_SET;//启动发送	   
	while(IRQ_S!=0);//等待发送完成
	sta=SPI_R_byte(STATUS);  //读取状态寄存器的值	   
	SPI_W_Reg(W_REGISTER+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)//达到最大重发次数
	{
		SPI_W_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}

//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
void NRF24L01_RX_Mode(void)
{
 
CE_CLR;  
  SPI_W_DBuffer(W_REGISTER+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址
	  
  SPI_W_Reg(W_REGISTER+EN_AA,0x01);    //使能通道0的自动应答    
  SPI_W_Reg(W_REGISTER+EN_RX_ADDR,0x01);//使能通道0的接收地址  	 
  SPI_W_Reg(W_REGISTER+RF_CH,40);	     //设置RF通信频率		  
  SPI_W_Reg(W_REGISTER+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	    
  SPI_W_Reg(W_REGISTER+RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  SPI_W_Reg(W_REGISTER+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
  CE_SET; //CE为高,进入接收模式 
}			


u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	sta=SPI_R_byte(STATUS);  //读取状态寄存器的值    	 
	SPI_W_Reg(W_REGISTER+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		SPI_R_DBuffer(R_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		SPI_W_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//没收到任何数据
}	







