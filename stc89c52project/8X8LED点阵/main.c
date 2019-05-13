#include <reg51.h>
#include <intrins.h>

typedef unsigned char u8;
typedef unsigned int  u16;

sbit SRCLK=P3^6;
sbit RCLK=P3^5;
sbit SER=P3^4;



void Hc595SendByte(u8 dat)
{
	u8 a;
	SRCLK=0;
	RCLK=0;
	for(a=0;a<8;a++)
	{
		SER=dat>>7;
		dat<<=1;

		SRCLK=1;
		_nop_();
		_nop_();
		SRCLK=0;	
	}

	RCLK=1;
	_nop_();
	_nop_();
	RCLK=0;
}

void main(){
	while(1){
		Hc595SendByte(0x80);
		P0=0x7f;	
	}
}