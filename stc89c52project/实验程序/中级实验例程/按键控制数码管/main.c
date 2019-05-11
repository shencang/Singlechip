

#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

sbit k1=P3^1;
sbit k2=P3^0;
sbit k3=P3^2;
sbit k4=P3^3;	 //���尴���˿�

u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//��ʾ0~F��ֵ


#define KEY1 1
#define KEY2 2
#define KEY3 3
#define KEY4 4

char keynum;
u8 DisplayData[8];
/*******************************************************************************
* �� �� ��         : delay
* ��������		   : ��ʱ������i=1ʱ����Լ��ʱ10us
*******************************************************************************/
void delay(u16 i)
{
	while(i--);	
}

//mode=0 ����   1������
u8 KeyScan(u8 mode)
{
	static u8 keyen=1;
	if(mode==1)
	{
		keyen=1;
	}
	if(keyen==1&&(k1==0||k2==0||k3==0||k4==0))
	{
		delay(1000);  //��������
		keyen=0;
		if(k1==0)return KEY1;
		else if(k2==0)return KEY2;
		else if(k3==0)return KEY3;
		else if(k4==0)return KEY4;				
	}
	else if(k1==1&&k2==1&&k3==1&&k4==1)
	{
		keyen=1;
	}
	return 0;	
}


/*******************************************************************************
* �� �� ��         : DigDisplay
* ��������		   : ����ܶ�̬ɨ�躯����ѭ��ɨ��8���������ʾ
*******************************************************************************/
void DigDisplay()
{
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)	 //λѡ��ѡ�����������ܣ�
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//��ʾ��0λ
			case(1):
				LSA=1;LSB=0;LSC=0; break;//��ʾ��1λ
			case(2):
				LSA=0;LSB=1;LSC=0; break;//��ʾ��2λ
			case(3):
				LSA=1;LSB=1;LSC=0; break;//��ʾ��3λ
			case(4):
				LSA=0;LSB=0;LSC=1; break;//��ʾ��4λ
			case(5):
				LSA=1;LSB=0;LSC=1; break;//��ʾ��5λ
			case(6):
				LSA=0;LSB=1;LSC=1; break;//��ʾ��6λ
			case(7):
				LSA=1;LSB=1;LSC=1; break;//��ʾ��7λ	
		}
		P0=DisplayData[i];//���Ͷ���
		delay(100); //���һ��ʱ��ɨ��	
		P0=0x00;//����
	}
}

void datapros()
{
	u8 key=KeyScan(0);
	switch(key)
	{
		case 1:  keynum++;if(keynum==100)keynum=0; break;
		case 2:  keynum--;if(keynum<=0)keynum=99; break;
		case 3:  keynum=0; break;
		case 4:   break;
	}
	DisplayData[0]=smgduan[keynum%10];
	DisplayData[1]=smgduan[keynum/10];
}
/*******************************************************************************
* �� �� ��       : main
* ��������		 : ������
* ��    ��       : ��
* ��    ��    	 : ��
*******************************************************************************/
void main()
{	
	while(1)
	{
		datapros();
		DigDisplay();	
	}		
}


