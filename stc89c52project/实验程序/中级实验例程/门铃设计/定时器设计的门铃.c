/* ���س���󰴼�K1���¾ͻᴥ�����������塰���ˡ���  */

#include<reg52.h>
typedef unsigned char u8;
typedef unsigned int u16;
sbit beep=P1^5;
sbit k1=P3^1;
u8 ding,dong,flag,stop;
u16 n;
void delay(u16 i)
{
	while(i--);
}
void time0init()	  //��ʱ��0��ʼ��
{
	TMOD=0X01;	 //��ʱ��0 ��ʽ1
	TH0=0Xff;
	TL0=0X06; //��ʱ250us
//	TR0=1;
	EA=1;
	ET0=1;
}
void biaohaoinit()	   //������ų�ʼ��
{
	ding=0;		//������  ������־
	dong=0;		//������  ������־
	n=0;		//��ʱ0.5s��־
	flag=0;
	stop=0;	   //������־
}
void main()
{
	time0init();
	biaohaoinit();
	while(1)
	{
		if(k1==0)	   //�жϰ����Ƿ���
		{
			delay(1000);  //����
			if(k1==0)
			{
				TR0=1;	//�򿪶�ʱ��0
				while(!stop);
			}
		}	
	}	
}
void time0() interrupt 1
{
	n++;
	TH0=0Xff;
	TL0=0X06; //250us
	if(n==2000)		//��ʱ0.5s  ����0.5�룬����0.5��
	{
		n=0;
		if(flag==0)
		{
			flag=~flag;
		}
		else
		{
			flag=0;
			stop=1;
			TR0=0;	  //�رն�ʱ��0
		}
	}
	if(flag==0)
	{					   //ͨ���ı䶨ʱ����ʱ����Ըı����������
		ding++;			  //��
		if(ding==1)
		{
			ding=0;
			beep=~beep;
		}
	}
	else
	{
		dong++;
		if(dong==2)		  //��
		{
			dong=0;
			beep=~beep;
		}	
	}
}