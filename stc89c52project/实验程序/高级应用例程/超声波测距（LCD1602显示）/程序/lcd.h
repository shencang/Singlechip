#ifndef __LCD_H_
#define __LCD_H_
/**********************************
当使用的是4位数据传输的时候定义，
使用8位取消这个定义
**********************************/
//#define LCD1602_4PINS

/**********************************
包含头文件
**********************************/
#include<reg51.h>

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

/**********************************
PIN口定义
**********************************/
#define LCD1602_DB  P0      //data bus 数据总线
sbit LCD1602_RS = P2^6;
sbit LCD1602_RW = P2^5;
sbit LCD1602_EN = P2^7;	 

/**********************************
函数声明
**********************************/
/*在51单片机12MHZ时钟下的延时函数*/
void Lcd1602_Delay1ms(uint c);   //误差 0us

void Read_Busy();           //忙检测函数，判断bit7是0，允许执行；1禁止
void Lcd1602_Write_Cmd(unsigned char cmd);     //写命令
void Lcd1602_Write_Data(unsigned char dat);   //写数据
void LcdSetCursor(unsigned char x,unsigned char y);  //坐标显示
void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str);     //显示字符串
void InitLcd1602();              //1602初始化
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData);
					  

#endif
