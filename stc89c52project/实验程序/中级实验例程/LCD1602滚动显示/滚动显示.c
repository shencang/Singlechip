#include<reg52.h>
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit rs=P2^6;
sbit rw=P2^5;
sbit e=P2^7;
uint8 a[16]="perchin designed";
uint8 b[27]="welcome to the world of mcu";
void delay(uint16 i)  //1us
{
	while(i--);
}
void wrc(uint8 c)
{
	delay(1000);
	rs=0;
	rw=0;
	e=0;
	P0=c;
	e=1;
	delay(10);
	e=0;
	/*
	P0=c<<4;
	e=1;
	e=0;*/       //四位的LCD要加上
}
void wrd(uint8 dat)
{
	delay(1000);
	rs=1;
	rw=0;
	e=0;
	P0=dat;
	e=1;
	delay(10);
	e=0;
	/*
	P0=dat<<4;
	e=1;
	e=0;*/	   //四位的LCD要加上
	rs=0;	
}
void init()
{
	delay(1000);
	/*wrc(0x32);	 
	wrc(0x28);
	wrc(0x28);	*/	//四位的LCD要加上
	wrc(0x38);
	wrc(0x38);
	wrc(0x38);	
	wrc(0x0c);	
	wrc(0x01);	
}
void display()
{
	uint8 i;
	wrc(0x00+0x80);
	for(i=0;i<16;i++)
	{
		wrd(a[i]);	
	}
	wrc(0x40+0x80);
	for(i=0;i<27;i++)
	{
		wrd(b[i]);
	} 
	wrc(0x07);	 //每写一个数据屏幕就要右移一位，就相对于数据来说就是左移了；
	while(1)
	{	
		wrc(0x00+0x80);
		for(i=0;i<16;i++)
		{
			wrd(a[i]);
			delay(30000);	//如果不加这条延时语句的话滚动会非常快。
		}
	}
}
void main()
{
	init();
	while(1)
	{
		display();
	}	
} 