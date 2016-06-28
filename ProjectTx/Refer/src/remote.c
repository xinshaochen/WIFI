#include "remote.h"
#include "ALL_Includes.h"


void Remote_Init(void) 
{

	sim();
	EXTI->CR1 |= BIT(6);
	EXTI->CR1 &= ~BIT(7);//CR1  �����ش���
	//EXTI->CR1 |= BIT(7);
	//EXTI->CR1 &= ~BIT(6);//CR1  �����ش���
	REMOTE->DDR &= ~REMOTE_P;
	REMOTE->CR1 |= REMOTE_P;
	REMOTE->CR2 |= REMOTE_P;




	TIM2->IER = 0x00;//��ֹ�ж�
	TIM2->EGR = 0x01;//����������±�־
	TIM2->PSCR = 0x04;//����ʱ�ӷ�Ƶ 2M/2=1MHz---1us
	TIM2->ARRH = 0x26;
	TIM2->ARRL = 0xA0;

	TIM2->CR1 |= 0x81;  //������ʱ��
	TIM2->IER |= 0x01;  //�����ж� 

	rim();       //rimʹ���ж�

}
//ң��������״̬
//[7]:�յ����������־
//[6]:�õ���һ��������������Ϣ
//[5]:����	
//[4]:����������Ƿ��Ѿ�������								   
//[3:0]:�����ʱ��
u8 	RmtSta = 0;
u16 Dval;		//�½���ʱ��������ֵ
u32 RmtRec = 0;	//������յ�������	   		    
u8  RmtCnt = 0;	//�������µĴ���	 
//��ʱ��2����ж�
#pragma vector=TIM2_Updata_vector
__interrupt void TIM2_Updata_IRQHandler(void)
{
	sim();//�����ж�
	if (RmtSta & 0x80)//�ϴ������ݱ����յ���
	{
		RmtSta &= ~0X10;						//ȡ���������Ѿ���������
		if ((RmtSta & 0X0F) == 0X00)RmtSta |= 1 << 6;//����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ�
		if ((RmtSta & 0X0F)<14)RmtSta++;
		else
		{
			RmtSta &= ~(1 << 7);//���������ʶ
			RmtSta &= 0XF0;	//��ռ�����	
		}
	}
	TIM2->SR1 &= ~0x01;  // ��������жϱ�ǣ��ⲽ����©������������������жϳ���
	rim();//�����ж�

}

#pragma   vector = EXTI3_PD_vector
__interrupt  void  EXTI3_PD_IRQHandler(void)
{
	if (REMOTE_S)
	{
		TIM2->CNTRL = 0;
		TIM2->CNTRH = 0;//��ն�ʱ��ֵ
						EXTI->CR1 |= BIT(7);
						EXTI->CR1 &= ~BIT(6);//CR1  �����ش���
		RmtSta |= 0X10;					//����������Ѿ�������
	}
	else
	{
		Dval = ((u16)TIM2->CNTRH << 8) | TIM2->CNTRL;
		EXTI->CR1 |= BIT(6);
		EXTI->CR1 &= ~BIT(7);//CR1  �����ش���
		if (RmtSta & 0X10)					//���һ�θߵ�ƽ���� 
		{
			if (RmtSta & 0X80)//���յ���������
			{

				if (Dval>300 && Dval<800)			//560Ϊ��׼ֵ,560us
				{
					RmtRec <<= 1;	//����һλ.
					RmtRec |= 0;	//���յ�0	   
				}
				else if (Dval>1400 && Dval<1800)	//1680Ϊ��׼ֵ,1680us
				{
					RmtRec <<= 1;	//����һλ.
					RmtRec |= 1;	//���յ�1
				}
				else if (Dval>2200 && Dval<2600)	//�õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms
				{
					RmtCnt++; 		//������������1��
					RmtSta &= 0XF0;	//��ռ�ʱ��		
				}
			}
			else if (Dval>4200 && Dval<4700)		//4500Ϊ��׼ֵ4.5ms
			{
				RmtSta |= 1 << 7;	//��ǳɹ����յ���������
				RmtCnt = 0;		//�����������������
			}
		}
		RmtSta &= ~(1 << 4);
	}
}

//����������
//����ֵ:
//	 0,û���κΰ�������
//����,���µİ�����ֵ.
u8 Remote_Scan(void)
{
	u8 sta = 0;
	u8 t1, t2;
	if (RmtSta&(1 << 6))//�õ�һ��������������Ϣ��
	{
		t1 = RmtRec >> 24;			//�õ���ַ��
		t2 = (RmtRec >> 16) & 0xff;	//�õ���ַ���� 
		if ((t1 == (u8)~t2) && t1 == REMOTE_ID)//����ң��ʶ����(ID)����ַ 
		{
			t1 = RmtRec >> 8;
			t2 = RmtRec;
			if (t1 == (u8)~t2)sta = t1;//��ֵ��ȷ	 
		}
		if ((sta == 0) || ((RmtSta & 0X80) == 0))//�������ݴ���/ң���Ѿ�û�а�����
		{
			RmtSta &= ~(1 << 6);//������յ���Ч������ʶ
			RmtCnt = 0;		//�����������������
		}
	}
	return sta;
}



