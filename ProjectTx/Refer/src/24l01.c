#include "24l01.h"
#include "ALL_Includes.h"
#include "delay.h"


const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ


uchar sta;    // ״̬���� 


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
//SPIд�Ĵ���һ���ֽ�
//reg:�Ĵ�����ַ
//value:ֵ
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
//��һ���Ĵ���
uchar SPI_R_byte(uchar reg)
{
  uchar reg_value;
  CSN_CLR;
  SPI_RW(reg);//��ַ
  reg_value=SPI_RW(0);//��ȡ
  CSN_SET;
  return reg_value;
}

//===================================
//SPI��ȡRXFIFO �Ĵ�������
//reg:�Ĵ�����ַ
//Dat_Buffer:��ȡ��������
//Dlen:���ݳ���
uchar SPI_R_DBuffer(uchar reg,uchar *Dat_Buffer,uchar Dlen)
{
  uchar status,i;
  CSN_CLR;
  status=SPI_RW(reg);//д�Ĵ�����ַ
  for(i=0;i<Dlen;i++)
  {
    Dat_Buffer[i]=SPI_RW(0);//ȡ����
  }
  CSN_SET;
  return status;
}
//=================================
//SPIдTXFIFO ����
//reg:д��Ĵ�����ַ
//TX_Dat_Buffer:Ҫ����������
//Dlen:���ݳ���
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



//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void NRF24L01_TX_Mode(void)
{														 
	CE_CLR;	    
  SPI_W_DBuffer(W_REGISTER+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  SPI_W_DBuffer(W_REGISTER+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  SPI_W_Reg(W_REGISTER+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  SPI_W_Reg(W_REGISTER+EN_RX_ADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  SPI_W_Reg(W_REGISTER+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  SPI_W_Reg(W_REGISTER+RF_CH,40);       //����RFͨ��Ϊ40
  SPI_W_Reg(W_REGISTER+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  SPI_W_Reg(W_REGISTER+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	CE_SET;//CEΪ��,10us����������
        
        delay_ms(1);
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
 	
	CE_CLR;
        SPI_W_DBuffer(W_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	CE_SET;//��������	   
	while(IRQ_S!=0);//�ȴ��������
	sta=SPI_R_byte(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	SPI_W_Reg(W_REGISTER+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		SPI_W_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void NRF24L01_RX_Mode(void)
{
 
CE_CLR;  
  SPI_W_DBuffer(W_REGISTER+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	  
  SPI_W_Reg(W_REGISTER+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
  SPI_W_Reg(W_REGISTER+EN_RX_ADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  SPI_W_Reg(W_REGISTER+RF_CH,40);	     //����RFͨ��Ƶ��		  
  SPI_W_Reg(W_REGISTER+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  SPI_W_Reg(W_REGISTER+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��   
  SPI_W_Reg(W_REGISTER+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  CE_SET; //CEΪ��,�������ģʽ 
}			


u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	sta=SPI_R_byte(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	SPI_W_Reg(W_REGISTER+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		SPI_R_DBuffer(R_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		SPI_W_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}	







