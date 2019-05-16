#include <reg52.h>
#include <intrins.h>

typedef unsigned char u8;
typedef unsigned int  u16;

sbit led = P2^2;
sbit sled= P2^1;
sbit k3 =P3^2;
sbit k4 =P3^3;

void delay(u16 i){
	while(i--);
}
void Int1Init(){
  IT1 =1;
	EX1= 1;
	EA = 1;
}
void Int0Init(){
  IT0 =1;
	EX0= 1;
	EA = 1;
}
	void main(){
	Int0Init();
	Int1Init();
	while(1);
		
}
	
void Int1() interrupt 2{
   delay(1000);
	if(k4==0){
		led= ~led;
	}
}
	void Int0() interrupt 0{
   delay(1000);
	if(k3==0){
		sled= ~sled;
	}
}