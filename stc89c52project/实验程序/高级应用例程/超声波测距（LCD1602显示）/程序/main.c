
#include<reg51.h>
#include<intrins.h>	
#include"lcd.h"

sbit Trig = P2^1;
sbit Echo = P2^0;



unsigned char PuZh[]=" Pechin Science ";
unsigned char code ASCII[15] =    {'0','1','2','3','4','5','6','7','8','9','.','-','M'};

static unsigned char DisNum = 0; //��ʾ��ָ��				  
       unsigned int  time=0;
	   unsigned long S=0;
	   bit      flag =0;
	   unsigned char disbuff[4]	   ={ 0,0,0,0,};
/*******************************************************************************
* �� �� ��         : main
* ��������		   : ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void Conut(void)
	{
	 time=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(time*1.7)/100;     //�������CM
	 if((S>=700)||flag==1) //����������Χ��ʾ��-��
	 {	 
	  flag=0;
	 
	  DisplayOneChar(0, 1, ASCII[11]);
	  DisplayOneChar(1, 1, ASCII[10]);	//��ʾ��
	  DisplayOneChar(2, 1, ASCII[11]);
	  DisplayOneChar(3, 1, ASCII[11]);
	  DisplayOneChar(4, 1, ASCII[12]);	//��ʾM
	 }
	 else
	 {
	  disbuff[0]=S%1000/100;
	  disbuff[1]=S%1000%100/10;
	  disbuff[2]=S%1000%10 %10;
	  DisplayOneChar(0, 1, ASCII[disbuff[0]]);
	  DisplayOneChar(1, 1, ASCII[10]);	//��ʾ��
	  DisplayOneChar(2, 1, ASCII[disbuff[1]]);
	  DisplayOneChar(3, 1, ASCII[disbuff[2]]);
	  DisplayOneChar(4, 1, ASCII[12]);	//��ʾM
	 }
	}

void zd0() interrupt 1 		 //T0�ж��������������,������෶Χ
  {
    flag=1;							 //�ж������־
  }

 void  StartModule() 		         //����ģ��
  {
	  Trig=1;			                     //����һ��ģ��
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  Trig=0;
  }



void delayms(unsigned int ms)
{
	unsigned char i=100,j;
	for(;ms;ms--)
	{
		while(--i)
		{
			j=10;
			while(--j);
		}
	}
}

void main(void)
{

	 TMOD=0x01;		   //��T0Ϊ��ʽ1��GATE=1��
	 TH0=0;
	 TL0=0;          
	 ET0=1;             //����T0�ж�
	 EA=1;			   //�������ж�	

	InitLcd1602();
	LcdShowStr(0,0,PuZh);
	while(1)
	{
		 StartModule();
	     while(!Echo);		//��RXΪ��ʱ�ȴ�
	     TR0=1;			    //��������
	     while(Echo);			//��RXΪ1�������ȴ�
	     TR0=0;				//�رռ���
         Conut();			//����
		 delayms(80);
	}
			
}