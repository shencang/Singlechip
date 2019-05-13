#include <reg52.h>
#include <intrins.h>

typedef unsigned char u8;
typedef unsigned int  u16;

sbit LSA  =P2^2;
sbit LSB  =P2^3;
sbit LSC  =P2^4;

#define GPIO_KEY P1
#define GPI0_DIG P0

u8 keyValue;

u8 code smgduan[]= {0x3f,0x06,0x5b,0x4f,
										0x66,0x6d,0x7d,0x07,
										0x7f,0x6f,0x77,0x7c,
										0x39,0x5e,0x79,0x71};
void delay(u16 i){
	while(i--);
}
	
void keyDown (){
	u8 a=0;
	GPIO_KEY= 0X0F;
	if(GPIO_KEY!=0X0F){
		delay(1000);
		if(GPIO_KEY != 0X0F ){
			GPIO_KEY = 0X0F;
			switch(GPIO_KEY){
				case (0x07): keyValue = 0; break;
				case (0x0b): keyValue = 1; break;
				case (0x0d): keyValue = 2; break;
				case (0x0e): keyValue = 3; break;
				 
			}GPIO_KEY  =0xF0;
			switch(GPIO_KEY){
				case (0x70): keyValue = keyValue; break;
				case (0xb0): keyValue = keyValue+4; break;
				case (0xd0): keyValue = keyValue+8; break;
				case (0xe0): keyValue = keyValue+12; break;
			}
			while((a<50)&&(GPIO_KEY!=0XF0)){
				delay(1000); 
				a++;
			}
	}
	
}
	}
void main(){
	LSA = 0;
	LSB = 1;
	LSC = 1;
	while(1){
			keyDown();
	GPI0_DIG = smgduan[keyValue];
	}
  
}