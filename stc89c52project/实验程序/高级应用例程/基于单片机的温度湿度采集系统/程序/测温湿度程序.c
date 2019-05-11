/**************************************************************** 

声明：本程序仅供大家参考学习，未经本人同意禁止用于商业用途。

***************************************************************/
#include <reg52.h>
#define uint unsigned int 
#define uchar unsigned char
#include <intrins.h>

typedef bit BOOL  ; 

sbit io = P1^1 ;//(口线定义)
sbit bee=P1^0;
sbit rs = P2^5 ; 
sbit rw = P2^6 ;
sbit ep = P2^7 ;
bit flag_300ms ;

sbit K1=P1^2;
sbit K2=P1^3;
sbit K3=P1^4;

uchar yemian=0;//（定义变量）
uchar flat=1;
uchar data_byte;
uchar RH,RL,TH,TL;
uchar WenDu,ShiDu;//报警的温度和湿度
#include "eeprom52.h"


/****************************延时程序****************************/
void delay(uchar ms)
{       // 延时子程序
	 uchar i ;
	 while(ms--)
	 {
	  	for(i = 0 ; i<250;i++) ;
	 }
}

void delay1()//延时10us
{
	uchar i;
	i--;
	i--;
	i--;
	i--;
	i--;
	i--;
}
void longdelay(uchar s) //长延时
{
	 while(s--)
	 {
	  	delay(20) ;
	 }
}



/******************把数据保存到单片机内部eeprom中******************/
void write_eeprom()
{
	SectorErase(0x2000);
 	byte_write(0x2000, WenDu);
	byte_write(0x2001, ShiDu);
	byte_write(0x2060, a_a);	
}

/******************把数据从单片机内部eeprom中读出来*****************/
void read_eeprom()
{
	WenDu   = byte_read(0x2000);
  ShiDu = byte_read(0x2001);
	a_a      = byte_read(0x2060);
}

/**************开机自检eeprom初始化*****************/
void init_eeprom()
{
	read_eeprom();		//先读
	if(a_a != 2)		//新的单片机初始单片机内问eeprom
	{
	WenDu   = 30;
	ShiDu   = 80;
		a_a = 2;
		write_eeprom();
	}	
}



/********************报警程序部分*******************/
void warn()  //报警函数
{
	
  if(TH>=WenDu)  //检测的温度高于设定温度报警值
		                             
     { 
      bee=0; //报警
			
     }      
		 
	if(RH>=ShiDu)  //检测的湿度高于设定的湿度值
		                             
     { 
      bee=0; //报警
     }      

		 
	if( TH<WenDu  &&  RH<ShiDu )	 
    {
		  bee=1; //停止报警
		}
		 
  
}

/************************LCD模块******************************************/

BOOL lcd_bz()//测试LCD忙碌状态
{      
	 BOOL result ;
	 rs = 0 ;
	 rw = 1 ;
	 ep = 1 ;
	 result = (BOOL)(P0 & 0x80) ;
	 ep = 0 ;
	 return result ; 
}

void write_cmd(uchar cmd)// 写指令
{       
 	while(lcd_bz()) ;
	rs = 0 ;
	rw = 0 ;
	ep = 0 ;
	P0 = cmd ;
	ep = 1 ;
	ep = 0 ;  
}

void write_addr(uchar addr)//写地址
{       
 	write_cmd(addr|0x80) ;
}

void write_byte(uchar dat)//写字节
{      
 	while(lcd_bz()) ;
  	rs = 1 ;
  	rw = 0 ;
  	ep = 0 ;
  	P0 = dat ;
  	ep = 1 ;
  	ep = 0 ; 
}

void lcd_init()// 初始化
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

void display(uchar addr, uchar q)//在某一地址上显示一字节
{ 	 
	 delay(1) ;
	 write_addr(addr) ;
   write_byte(q) ;
	 delay(1) ;
	 
}

/*********************** DHT11测试模块***************************************/

void start()//开始信号
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
uchar receive_byte()//接收一个字节
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
void receive()//接收数据
{
	uchar T_H,T_L,R_H,R_L,check,num_check;
	uchar count;
	start();//开始信号
	io=1;
	if(!io)//读取DHT11响应信号
	{
	
	while((io)&&count++);
	R_H=receive_byte();
	R_L=receive_byte();
	T_H=receive_byte();
	T_L=receive_byte();
	check=receive_byte();
	io=0;//拉低延时50us
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


void showw()   // 问候语函数
{      
	   lcd_init(); //初始化
	    delay(10);
	
     display(0x05,'W');
     display(0x06,'e');
		 display(0x07,'l');
		 display(0x08,'c');
		 display(0x09,'o');
		 display(0x0a,'m');
     display(0x0b,'e');
	   delay(250); //短暂延时
	   delay(250);
	   delay(250);
	   

/*********************************/
/*
	   display(0x40,' ') ;
		 display(0x41,'h');
		 display(0x42,'e');
		
 */
}


/*************定时器0初始化程序***************/
void time_init()	  
{
	EA   = 1;	 	  //开总中断
	TMOD = 0X01;	  //定时器0、定时器1工作方式1
	ET0  = 1;		  //开定时器0中断 
	TR0  = 1;		  //允许定时器0定时
}



void key() //按键程序 选择菜单模式
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


shezhi()  //对里面的数值进行修改
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
								if(WenDu>99) { WenDu=99; }  //湿度上限加
							 }
							 
							 if(yemian==2)
							 {
							  ShiDu+=1;
								if(ShiDu>99) { ShiDu=99; }  //湿度下限加
							 }
							 
								write_eeprom();       //保存数据		
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
							      WenDu-=1;          //湿度下限减
								    if(WenDu<1) {WenDu=1; } 
							     }
							 
							   if(yemian==2)
							     {
							      ShiDu-=1;         //湿度下限减
								    if(ShiDu<1) { ShiDu=1; } 
							     }
								write_eeprom();       //保存数据		
             }
		   }

}



/***************************主程序*********************************/
void main()
{
	 init_eeprom();      			//读eeprom数据
   time_init();				//初始化定时器 
   showw();    //显示问候语 和名字
  
	while(1)
	{   
		
		   key();
		//====================yemian==0时 进入工作状态=================================
		 	if(yemian==0)  
			{ 
				if(flat==1)
				 {
           flat=0;
					 lcd_init(); //初始化
					 	 
					 display(0x00,'T') ;    //Temp
		       display(0x01,'e');
		       display(0x02,'m');
					 display(0x03,'p');
           display(0x04,':');
					 
					 display(0x08,0xdf); // 度
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
           display(0x0e,WenDu/10+0x30);  //报警温度
		       display(0x0f,WenDu%10+0x30);
				   
					 display(0x4c,'H');
           display(0x4d,':');
				   display(0x4e,ShiDu/10+0x30);  //报警湿度
		       display(0x4f,ShiDu%10+0x30);
          
          }
			
			 if(flag_300ms == 1)
	    	 {		
			     flag_300ms = 0;
           receive();
					 
					 display(0x05,TH/10+0x30);  //温度显示
		       display(0x06,TH%10+0x30);
					 
           display(0x45,RH/10+0x30);  //湿度显示
		       display(0x46,RH%10+0x30);
					 
					
           warn();
		     }
			
	}		
			
	//====================yemian==1 温度设置===========================
				if(yemian==1)  //温度
		    	{
		         if(flat==1)
				     {
               flat=0;  
							  bee=1;
               lcd_init(); //初始化
							 display(0x06,'T');
							 display(0x07,'e');
							 display(0x08,'m');
							 display(0x09,'p');
             }
						 
						shezhi();
						display(0x47,WenDu/10+0x30);  //温度
		        display(0x48,WenDu%10+0x30);
		      }
			
		//====================yemian==1湿度设置==========================	
				if(yemian==2)   //湿度设置
			    {
		         if(flat==1)
				     {
               flat=0;  
							  bee=1;
               lcd_init(); //初始化
							 display(0x06,'H');
							 display(0x07,'u');
							 display(0x08,'m');
							 display(0x09,'i');
						 } 
						 shezhi();
						 display(0x47,ShiDu/10+0x30);  //湿度
		         display(0x48,ShiDu%10+0x30);
		      }
	}
}



/*************定时器0中断服务程序***************/
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
