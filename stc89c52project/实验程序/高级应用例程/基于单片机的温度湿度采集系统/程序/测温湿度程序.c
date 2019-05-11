/**************************************************************** 

�����������������Ҳο�ѧϰ��δ������ͬ���ֹ������ҵ��;��

***************************************************************/
#include <reg52.h>
#define uint unsigned int 
#define uchar unsigned char
#include <intrins.h>

typedef bit BOOL  ; 

sbit io = P1^1 ;//(���߶���)
sbit bee=P1^0;
sbit rs = P2^5 ; 
sbit rw = P2^6 ;
sbit ep = P2^7 ;
bit flag_300ms ;

sbit K1=P1^2;
sbit K2=P1^3;
sbit K3=P1^4;

uchar yemian=0;//�����������
uchar flat=1;
uchar data_byte;
uchar RH,RL,TH,TL;
uchar WenDu,ShiDu;//�������¶Ⱥ�ʪ��
#include "eeprom52.h"


/****************************��ʱ����****************************/
void delay(uchar ms)
{       // ��ʱ�ӳ���
	 uchar i ;
	 while(ms--)
	 {
	  	for(i = 0 ; i<250;i++) ;
	 }
}

void delay1()//��ʱ10us
{
	uchar i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
}
void longdelay(uchar s) //����ʱ
{
	 while(s--)
	 {
	  	delay(20) ;
	 }
}



/******************�����ݱ��浽��Ƭ���ڲ�eeprom��******************/
void write_eeprom()
{
	SectorErase(0x2000);
 	byte_write(0x2000, WenDu);
	byte_write(0x2001, ShiDu);
	byte_write(0x2060, a_a);	
}

/******************�����ݴӵ�Ƭ���ڲ�eeprom�ж�����*****************/
void read_eeprom()
{
	WenDu   = byte_read(0x2000);
  ShiDu = byte_read(0x2001);
	a_a      = byte_read(0x2060);
}

/**************�����Լ�eeprom��ʼ��*****************/
void init_eeprom()
{
	read_eeprom();		//�ȶ�
	if(a_a != 2)		//�µĵ�Ƭ����ʼ��Ƭ������eeprom
	{
	WenDu   = 30;
	ShiDu   = 80;
		a_a = 2;
		write_eeprom();
	}	
}



/********************�������򲿷�*******************/
void warn()  //��������
{
	
  if(TH>=WenDu)  //�����¶ȸ����趨�¶ȱ���ֵ
		                             
     { 
      bee=0; //����
			
     }      
		 
	if(RH>=ShiDu)  //����ʪ�ȸ����趨��ʪ��ֵ
		                             
     { 
      bee=0; //����
     }      

		 
	if( TH<WenDu  &&  RH<ShiDu )	 
    {
		  bee=1; //ֹͣ����
		}
		 
  
}

/************************LCDģ��******************************************/

BOOL lcd_bz()//����LCDæµ״̬
{      
	 BOOL result ;
	 rs = 0 ;
	 rw = 1 ;
	 ep = 1 ;
	 result = (BOOL)(P0 & 0x80) ;
	 ep = 0 ;
	 return result ; 
}

void write_cmd(uchar cmd)// дָ��
{       
 	while(lcd_bz()) ;
	rs = 0 ;
	rw = 0 ;
	ep = 0 ;
	P0 = cmd ;
	ep = 1 ;
	ep = 0 ;  
}

void write_addr(uchar addr)//д��ַ
{       
 	write_cmd(addr|0x80) ;
}

void write_byte(uchar dat)//д�ֽ�
{      
 	while(lcd_bz()) ;
  	rs = 1 ;
  	rw = 0 ;
  	ep = 0 ;
  	P0 = dat ;
  	ep = 1 ;
  	ep = 0 ; 
}

void lcd_init()// ��ʼ��
{       
	 write_cmd(0x38) ; 
	 delay(1);
	 write_cmd(0x08) ;  
	 delay(1);
	 write_cmd(0x01) ; 
	 delay(1);
	 write_cmd(0x06) ;
	 delay(1);
	 write_cmd(0x0c) ; 
	 delay(1);
}

void display(uchar addr, uchar q)//��ĳһ��ַ����ʾһ�ֽ�
{ 	 
	 delay(1) ;
	 write_addr(addr) ;
   write_byte(q) ;
	 delay(1) ;
	 
}

/*********************** DHT11����ģ��***************************************/

void start()//��ʼ�ź�
{
	io=1;
	delay1();
	io=0;
	delay(20);//>18ms
	io=1;
	delay1();//20-40us
	delay1();
	delay1();
	delay1();
	delay1();
}
uchar receive_byte()//����һ���ֽ�
{
	uchar i,temp,count;
	for(i=0;i<8;i++)
	{
        temp=0;
		delay1();delay1();delay1();delay1();
		if(io==1)temp=1;
		count=2;
		while((io)&&count++);
		if(count==1)break;
		data_byte<<=1;	
		data_byte|=temp;
	}
	return data_byte;
}
void receive()//��������
{
	uchar T_H,T_L,R_H,R_L,check,num_check;
	uchar count;
	start();//��ʼ�ź�
	io=1;
	if(!io)//��ȡDHT11��Ӧ�ź�
	{
	
	while((io)&&count++);
	R_H=receive_byte();
	R_L=receive_byte();
	T_H=receive_byte();
	T_L=receive_byte();
	check=receive_byte();
	io=0;//������ʱ50us
	delay1();delay1();delay1();delay1();delay1();
	io=1;
	num_check=R_H+R_L+T_H+T_L;
	if(num_check=check)
	{
		RH=R_H;
		RL=R_L;
		TH=T_H;
		TL=T_L;
		check=num_check;
	}
	}
}


void showw()   // �ʺ��ﺯ��
{      
	   lcd_init(); //��ʼ��
	    delay(10);
	
     display(0x05,'W');
     display(0x06,'e');
		 display(0x07,'l');
		 display(0x08,'c');
		 display(0x09,'o');
		 display(0x0a,'m');
     display(0x0b,'e');
	   delay(250); //������ʱ
	   delay(250);
	   delay(250);
	   

/*********************************/
/*
	   display(0x40,' ') ;
		 display(0x41,'h');
		 display(0x42,'e');
		
 */
}


/*************��ʱ��0��ʼ������***************/
void time_init()	  
{
	EA   = 1;	 	  //�����ж�
	TMOD = 0X01;	  //��ʱ��0����ʱ��1������ʽ1
	ET0  = 1;		  //����ʱ��0�ж� 
	TR0  = 1;		  //����ʱ��0��ʱ
}



void key() //�������� ѡ��˵�ģʽ
{
     
	  if(!K1)
		   {  
			  delay(20);
			    if(!K1)
		         {   
							 while(!K1)
				           ;
							  yemian++;
							  flat++;
								if(yemian>=3) { yemian=0; } 
								
             }
		   }
 
	
	
}


shezhi()  //���������ֵ�����޸�
{
    if(!K2)
		   {  
			  delay(50);
			    if(!K2)
		         {   
							 ;
							 
							 if(yemian==1)
							 {
							  WenDu+=1;
								if(WenDu>99) { WenDu=99; }  //ʪ�����޼�
							 }
							 
							 if(yemian==2)
							 {
							  ShiDu+=1;
								if(ShiDu>99) { ShiDu=99; }  //ʪ�����޼�
							 }
							 
								write_eeprom();       //��������		
             }
		   }
			 
			  if(!K3)
		   {  
			    delay(50);
			     if(!K3)
		         {   
							
				           ;
							   if(yemian==1)
							     {
							      WenDu-=1;          //ʪ�����޼�
								    if(WenDu<1) {WenDu=1; } 
							     }
							 
							   if(yemian==2)
							     {
							      ShiDu-=1;         //ʪ�����޼�
								    if(ShiDu<1) { ShiDu=1; } 
							     }
								write_eeprom();       //��������		
             }
		   }

}



/***************************������*********************************/
void main()
{
	 init_eeprom();      			//��eeprom����
   time_init();				//��ʼ����ʱ�� 
   showw();    //��ʾ�ʺ��� ������
  
	while(1)
	{   
		
		   key();
		//====================yemian==0ʱ ���빤��״̬=================================
		 	if(yemian==0)  
			{ 
				if(flat==1)
				 {
           flat=0;
					 lcd_init(); //��ʼ��
					 	 
					 display(0x00,'T') ;    //Temp
		       display(0x01,'e');
		       display(0x02,'m');
					 display(0x03,'p');
           display(0x04,':');
					 
					 display(0x08,0xdf); // ��
		       display(0x09,'C');
					 
					 
					 display(0x40,'H') ;    //Humi
		       display(0x41,'u');
		       display(0x42,'m');
					 display(0x43,'i');
           display(0x44,':');
					 
					 display(0x47,'%');
					 display(0x48,'R');
					 display(0x49,'H');
					 
					 display(0x0c,'T');
           display(0x0d,':');
           display(0x0e,WenDu/10+0x30);  //�����¶�
		       display(0x0f,WenDu%10+0x30);
				   
					 display(0x4c,'H');
           display(0x4d,':');
				   display(0x4e,ShiDu/10+0x30);  //����ʪ��
		       display(0x4f,ShiDu%10+0x30);
          
          }
			
			 if(flag_300ms == 1)
	    	 {		
			     flag_300ms = 0;
           receive();
					 
					 display(0x05,TH/10+0x30);  //�¶���ʾ
		       display(0x06,TH%10+0x30);
					 
           display(0x45,RH/10+0x30);  //ʪ����ʾ
		       display(0x46,RH%10+0x30);
					 
					
           warn();
		     }
			
	}		
			
	//====================yemian==1 �¶�����===========================
				if(yemian==1)  //�¶�
		    	{
		         if(flat==1)
				     {
               flat=0;  
							  bee=1;
               lcd_init(); //��ʼ��
							 display(0x06,'T');
							 display(0x07,'e');
							 display(0x08,'m');
							 display(0x09,'p');
             }
						 
						shezhi();
						display(0x47,WenDu/10+0x30);  //�¶�
		        display(0x48,WenDu%10+0x30);
		      }
			
		//====================yemian==1ʪ������==========================	
				if(yemian==2)   //ʪ������
			    {
		         if(flat==1)
				     {
               flat=0;  
							  bee=1;
               lcd_init(); //��ʼ��
							 display(0x06,'H');
							 display(0x07,'u');
							 display(0x08,'m');
							 display(0x09,'i');
						 } 
						 shezhi();
						 display(0x47,ShiDu/10+0x30);  //ʪ��
		         display(0x48,ShiDu%10+0x30);
		      }
	}
}



/*************��ʱ��0�жϷ������***************/
void time0_int() interrupt 1
{	
	static uchar value;
	TH0 = 0x3c;
	TL0 = 0xb0;     // 50ms
	 
	if(value % 6 == 0)
	{
		flag_300ms = 1;	   //300ms
		value = 0;
	}
}
