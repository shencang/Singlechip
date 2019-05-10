#include <reg52.h> 
#include <intrins.h>
typedef unsigned char u8;
typedef unsigned int u16;
#define led P2
sbit beep = P1^5;

void delay(u16 i){
  while(i--);
}
void main(void){
	 while(1){
		 beep = ~beep;
		 delay(500);
	 }		 
}