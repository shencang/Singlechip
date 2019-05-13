#include <reg52.h>
#include <intrins.h>

typedef unsigned char u8;
typedef unsigned int  u16;

sbit led = P2^2;
sbit k1 =P3^1;
sbit k2 =P3^0;
sbit k3 =P3^2;
sbit k4 =P3^3;
void delay(u16 i){
	while(i--);
}
	
void keypros(){
	if(k1==0){
	  delay(1000);
		if(k1==0){
		  led = ~led;
		}
		while(!k1);
	}
	if(k2==0){
	  delay(1000);
		if(k2==0){
		  led = ~led;
		}
		while(!k2);
	}
	if(k3==0){
	  delay(1000);
		if(k3==0){
		  led = ~led;
		}
		while(!k3);
	}
	if(k4==0){
	  delay(1000);
		if(k4==0){
		  led = ~led;
		}
		while(!k4);
	}
}

void main(){
	while(1){
		keypros();
	}
}