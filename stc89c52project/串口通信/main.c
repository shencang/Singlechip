#include <reg52.h> 
#include <intrins.h>
typedef unsigned char u8;
typedef unsigned int u16;


void UsartInit(){
		  TMOD = 0x20;
			SCON = 0x50;
			TH1 = 0xF3;
			TL1 = TH1;
			PCON = 0x80;
			EA = 1;
			ES = 1;
			TR1 = 1;
		 
}

void main(void){
	UsartInit();
	while(1);
	
	}

	
	void Usart() interrupt 4{
		u8 receiveData;
		receiveData = SBUF;
		RI =0;
		SBUF = receiveData;
		while (!TI);
		TI=0;
		
	}