#include<reg52.h>
#include "XPT2046.h"

typedef unsigned char uint8;
typedef unsigned int uint16;
sbit rs=P2^6;	 // ��������ѡ��
sbit rw=P2^5;	 //��дѡ��
sbit e=P2^7;	  //ʹ��

sbit k1=P3^3; //ģʽ
sbit k2=P2^1; //��
sbit k3=P2^2; //��

sbit moto=P3^6;
sbit beep=P3^7;


uint8 mode=0;
char temph=20;
uint8 temp,flag;

uint8 code num[10]="0123456789";

uint8 code str1[]="GAS:";  
uint8 code str2[]="SET:"; 



void delay(uint16 i)
{
	while(i--);
}

void wrc(uint8 c)	 //д����
{
	delay(1000);
	rs=0;
	rw=0;
	e=0;
	P0=c;
	e=1;
	delay(10);
	e=0;
}
void wrd(uint8 dat)	  //д����
{
	delay(1000);
	rs=1;
	rw=0;
	e=0;
	P0=dat;
	e=1;
	delay(10);
	e=0;
	rs=0;
}
void lcd_init()	   // LCD1602��ʼ��
{
	delay(1000);
	wrc(0x38);
	wrc(0x38);	 //�����������ѡ��8λ���ߣ�˫����ʾ  5*7�����ַ�
	wrc(0x38);
	wrc(0x06);	//������ʾģʽ����  �������  �������ƶ�
	wrc(0x0c);	//��ʾ���ؿ���  ����ʾ  �޹�� ��겻��˸
	wrc(0x01);	//����ָ��  �̶���
}

void key_pros()  //����������
{
	if(k1==0)
	{
		delay(1000);
		if(k1==0)
		{
			mode++;
			flag=0;
			if(mode==2)
			{
				mode=0;
				flag=1;	
			}
			
		}
		while(!k1);
	}
	if(mode==1)		  //�Կ�����ⱨ��ֵ�趨
	{
		if(k2==0)		  //��
		{
			delay(1000);
			if(k2==0)
			{
				temph+=1;
				if(temph>=100)temph=100;
			}
			while(!k2);
		}
		if(k3==0)	   //��
		{
			delay(1000);
			if(k3==0)
			{
				temph-=1;
				if(temph<=0)temph=0;	
			}
			while(!k3);
		}
	}
	
}

void lcd_init_display()	   //LCD��ʼ����ʾ
{
	uint8 i;
	for(i=0;i<4;i++)
	{
		wrc(0x80+i);
		wrd(str1[i]);	
	}

	for(i=0;i<4;i++)
	{
		wrc(0xc0+i);
		wrd(str2[i]);	
	}
}

void data_pros()	//���ݴ�����
{
	uint8 tempbuf[3],temp_buf[3],i;
	tempbuf[0]=temp/100+0x30;
	tempbuf[1]=temp%100/10+0x30;				
	tempbuf[2]=temp%100%10+0x30;
	
	temp_buf[0]=temph/100+0x30;
	temp_buf[1]=temph%100/10+0x30;				
	temp_buf[2]=temph%100%10+0x30;

	for(i=0;i<3;i++)
	{
		wrc(0x84+i);
		wrd(tempbuf[i]);
	}

	for(i=0;i<3;i++)
	{
		wrc(0xc4+i);
		wrd(temp_buf[i]);
	}
}

void sound()
{
	uint8 i=100;
	while(i--)
	{
		beep=~beep;
		delay(100);	
	}
}
void baojinpros()	//��������
{
	if(temp>=temph&&flag==1)
	{
		moto=0;
		sound();
	}
	else
	{	
		moto=1;
		beep=1;
	}
}

void main()
{
	moto=1;
	beep=1;
	lcd_init();
	lcd_init_display();	   //LCD��ʼ����ʾ
	while(1)
	{	
		key_pros();
		data_pros();
		baojinpros();
		temp = Read_AD_Data(0x94)/40;			
	}	
}
