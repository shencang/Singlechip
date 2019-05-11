

#include "reg52.h"			 //此文件中定义了单片机的一些特殊功能寄存器

typedef unsigned int u16;	  //对数据类型进行声明定义
typedef unsigned char u8;

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

sbit k1=P3^1;
sbit k2=P3^0;
sbit k3=P3^2;
sbit k4=P3^3;	 //定义按键端口

u8 code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//显示0~F的值


#define KEY1 1
#define KEY2 2
#define KEY3 3
#define KEY4 4

char keynum;
u8 DisplayData[8];
/*******************************************************************************
* 函 数 名         : delay
* 函数功能		   : 延时函数，i=1时，大约延时10us
*******************************************************************************/
void delay(u16 i)
{
	while(i--);	
}

//mode=0 单次   1：连续
u8 KeyScan(u8 mode)
{
	static u8 keyen=1;
	if(mode==1)
	{
		keyen=1;
	}
	if(keyen==1&&(k1==0||k2==0||k3==0||k4==0))
	{
		delay(1000);  //消抖处理
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
* 函 数 名         : DigDisplay
* 函数功能		   : 数码管动态扫描函数，循环扫描8个数码管显示
*******************************************************************************/
void DigDisplay()
{
	u8 i;
	for(i=0;i<8;i++)
	{
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;//显示第0位
			case(1):
				LSA=1;LSB=0;LSC=0; break;//显示第1位
			case(2):
				LSA=0;LSB=1;LSC=0; break;//显示第2位
			case(3):
				LSA=1;LSB=1;LSC=0; break;//显示第3位
			case(4):
				LSA=0;LSB=0;LSC=1; break;//显示第4位
			case(5):
				LSA=1;LSB=0;LSC=1; break;//显示第5位
			case(6):
				LSA=0;LSB=1;LSC=1; break;//显示第6位
			case(7):
				LSA=1;LSB=1;LSC=1; break;//显示第7位	
		}
		P0=DisplayData[i];//发送段码
		delay(100); //间隔一段时间扫描	
		P0=0x00;//消隐
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
* 函 数 名       : main
* 函数功能		 : 主函数
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void main()
{	
	while(1)
	{
		datapros();
		DigDisplay();	
	}		
}


