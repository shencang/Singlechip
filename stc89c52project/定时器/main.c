#include <reg52.h> 
#include <intrins.h>
typedef unsigned char u8;
typedef unsigned int u16;
#define led P2
sbit led2 = P2^0;

void delay(u16 i){
  while(i--);
}

void Timer1Init(){
	TMOD  |=  0x10;
	TH1 = 0XFC;
	TL1 = 0X18;
	ET1 = 1;
  EA  = 1;
  TR1 = 1;
	
}
void main(void){
	Timer1Init();
	while(1);
	
	}

	void Time1() interrupt 3 {
		static u16 i;
		TH1 = 0XFC;
		TL1 = 0X18;
		i++;
		if(i==1000){
			i=0;
			led2 = ~led2;
		}
	}