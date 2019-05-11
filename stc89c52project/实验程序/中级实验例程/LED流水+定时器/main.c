
#include<reg51.h>				 
//宏定义
#define uchar unsigned char 
#define LED P2		

//全局变量
uchar Time;

void main(void)
{
	uchar data1=0xfe;
	TMOD = 0x01; //选择工作方式1
  TH0 = 0x3C;	 //设置初始值,定时50MS
  TL0 = 0xB0; 
  EA = 1;			 //打开总中断
  ET0 = 1;		 //打开定时器0中断
  TR0 = 1;		 //启动定时器0
	while(1)
	{
		LED = data1;
		if(Time==10) 			//0.5s进入if内进行移位
		{
			Time = 0;
			data1 = data1<<1|0x01;	
			if(data1==0xff)data1=0xfe;
		}
		
		
	}	
}

void Timer0() interrupt 1
{
	  TH0 = 0x3C;	 //设置初始值
    TL0 = 0xB0;
		Time++;
}

