/* 下载程序后按键K1按下就会触发蜂鸣器门铃“叮咚”响  */

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
void time0init()	  //定时器0初始化
{
	TMOD=0X01;	 //定时器0 方式1
	TH0=0Xff;
	TL0=0X06; //定时250us
//	TR0=1;
	EA=1;
	ET0=1;
}
void biaohaoinit()	   //各个标号初始化
{
	ding=0;		//叮声音  计数标志
	dong=0;		//咚声音  计数标志
	n=0;		//定时0.5s标志
	flag=0;
	stop=0;	   //结束标志
}
void main()
{
	time0init();
	biaohaoinit();
	while(1)
	{
		if(k1==0)	   //判断按键是否按下
		{
			delay(1000);  //消抖
			if(k1==0)
			{
				TR0=1;	//打开定时器0
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
	if(n==2000)		//定时0.5s  叮响0.5秒，咚响0.5秒
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
			TR0=0;	  //关闭定时器0
		}
	}
	if(flag==0)
	{					   //通过改变定时计数时间可以改变门铃的声音
		ding++;			  //叮
		if(ding==1)
		{
			ding=0;
			beep=~beep;
		}
	}
	else
	{
		dong++;
		if(dong==2)		  //咚
		{
			dong=0;
			beep=~beep;
		}	
	}
}