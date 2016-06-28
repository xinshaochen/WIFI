#ifndef _24L01_H_
#define _24L01_H_
#include "stm8s.h"

#define   uint    u16
#define   uchar   u8


//========================================
//���Ŷ���
#define CE GPIOC
#define CE_P (1<<3)
#define CSN GPIOC
#define CSN_P (1<<4)
#define SCK GPIOC
#define SCK_P (1<<5)
#define MOSI GPIOC
#define MOSI_P (1<<6)
#define MISO GPIOC
#define MISO_P (1<<7)
#define IRQ GPIOD
#define IRQ_P (1<<3)

#define CE_SET CE->ODR|=CE_P
#define CE_CLR CE->ODR&=~CE_P
#define CSN_SET CSN->ODR|=CSN_P
#define CSN_CLR CSN->ODR&=~CSN_P
#define SCK_SET SCK->ODR|=SCK_P
#define SCK_CLR SCK->ODR&=~SCK_P
#define MOSI_SET MOSI->ODR|=MOSI_P
#define MOSI_CLR MOSI->ODR&=~MOSI_P

#define MISO_S ((MISO->IDR&MISO_P)>>7)
#define IRQ_S  ((IRQ->IDR&IRQ_P)>>3)




/*****************************************************************
nRF24L01ָ���ʽ
*******************************************************************/
#define R_REGISTER    0x00//���Ĵ���
#define W_REGISTER    0x20//д�Ĵ���
#define R_RX_PLOAD    0x61//��RX FIFO��Ч���ݣ�1-32�ֽڣ�����������ɺ����ݱ������Ӧ���ڽ���ģʽ 
#define W_TX_PLOAD    0xA0//дTX FIFO��Ч���ݣ�1-32�ֽڣ�д�������ֽ�0��ʼ��Ӧ���ڷ���ģʽ 
#define FLUSH_TX      0xE1//���TX FIFO�Ĵ�����Ӧ���ڷ���ģʽ 
#define FLUSH_RX      0xE2//���RX FIFO�Ĵ�����Ӧ���ڽ���ģʽ 
#define REUSE_TX_PL   0xE3//����ʹ����һ����Ч���ݣ���CEΪ�߹����У����ݰ������ϵ����·��� 
#define NOP           0xFF//�ղ���������������״̬�Ĵ���


/******************************************************************
nRF24L01�Ĵ�����ַ
*******************************************************************/

#define CONFIG      0x00  // ���üĴ��� 
#define EN_AA       0x01  // ���Զ�Ӧ�𡱹��ܼĴ� 
#define EN_RX_ADDR  0x02  // ����ͨ��ʹ�ܼĴ��� 
#define SETUP_AW    0x03  // ��ַ������üĴ��� 
#define SETUP_RETR  0x04  // �Զ��ط����üĴ��� 
#define RF_CH       0x05  // ��Ƶͨ��Ƶ�����üĴ��� 
#define RF_SETUP    0x06  // ��Ƶ���üĴ��� 
#define STATUS      0x07  // ״̬�Ĵ��� 
#define OBSERVE_TX  0x08  // ���ͼ��Ĵ���

#define CD          0x09  // �ز����Ĵ��� 
#define RX_ADDR_P0  0x0A  // ����ͨ��0���յ�ַ�Ĵ��� 
#define RX_ADDR_P1  0x0B  // ����ͨ��1���յ�ַ�Ĵ��� 
#define RX_ADDR_P2  0x0C  // ����ͨ��2���յ�ַ�Ĵ��� 
#define RX_ADDR_P3  0x0D  // ����ͨ��3���յ�ַ�Ĵ��� 
#define RX_ADDR_P4  0x0E  // ����ͨ��4���յ�ַ�Ĵ��� 
#define RX_ADDR_P5  0x0F  // ����ͨ��5���յ�ַ�Ĵ��� 
#define TX_ADDR     0x10  // ���͵�ַ�Ĵ��� 
#define RX_PW_P0    0x11  // ����ͨ��0��Ч���ݿ�����üĴ��� 
#define RX_PW_P1    0x12  // ����ͨ��1��Ч���ݿ�����üĴ��� 
#define RX_PW_P2    0x13  // ����ͨ��2��Ч���ݿ�����üĴ��� 
#define RX_PW_P3    0x14  // ����ͨ��3��Ч���ݿ�����üĴ��� 
#define RX_PW_P4    0x15  // ����ͨ��4��Ч���ݿ�����üĴ��� 
#define RX_PW_P5    0x16  // ����ͨ��5��Ч���ݿ�����üĴ��� 
#define FIFO_STATUS 0x17  // FIFO״̬�Ĵ���


#define MAX_TX  		0x10  //�ﵽ����ʹ����ж�
#define TX_OK   		0x20  //TX��������ж�
#define RX_OK   		0x40  //���յ������ж�




//24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��


void NRF24L01_Init(void);
uchar SPI_RW(uchar byte);
uchar SPI_W_Reg(uchar reg,uchar value);
uchar SPI_R_byte(uchar reg);
uchar SPI_R_DBuffer(uchar reg,uchar *Dat_Buffer,uchar Dlen);
uchar SPI_W_DBuffer(uchar reg,uchar *TX_Dat_Buffer,uchar Dlen);

uchar NRF24L01_Check(void);

void NRF24L01_TX_Mode(void);
/// <summary>
/// ���ͺ���
/// </summary>
/// <param name="txbuf">�ַ���</param>
u8 NRF24L01_TxPacket(u8 *txbuf);

void NRF24L01_RX_Mode(void);

u8 NRF24L01_RxPacket(u8 *rxbuf);


#endif
