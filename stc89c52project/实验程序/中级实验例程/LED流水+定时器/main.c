
#include<reg51.h>				 
//�궨��
#define uchar unsigned char 
#define LED P2		

//ȫ�ֱ���
uchar Time;

void main(void)
{
	uchar data1=0xfe;
	TMOD = 0x01; //ѡ������ʽ1
  TH0 = 0x3C;	 //���ó�ʼֵ,��ʱ50MS
  TL0 = 0xB0; 
  EA = 1;			 //�����ж�
  ET0 = 1;		 //�򿪶�ʱ��0�ж�
  TR0 = 1;		 //������ʱ��0
	while(1)
	{
		LED = data1;
		if(Time==10) 			//0.5s����if�ڽ�����λ
		{
			Time = 0;
			data1 = data1<<1|0x01;	
			if(data1==0xff)data1=0xfe;
		}
		
		
	}	
}

void Timer0() interrupt 1
{
	  TH0 = 0x3C;	 //���ó�ʼֵ
    TL0 = 0xB0;
		Time++;
}

