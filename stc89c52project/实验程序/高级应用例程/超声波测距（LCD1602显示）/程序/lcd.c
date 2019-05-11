#include"lcd.h"


void Read_Busy()           //æ��⺯�����ж�bit7��0������ִ�У�1��ֹ
{
    unsigned char sta;      //
    LCD1602_DB = 0xff;
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do
    {
        LCD1602_EN = 1;
        sta = LCD1602_DB;
        LCD1602_EN = 0;    //ʹ�ܣ���������ͣ��ͷ�����
    }while(sta & 0x80);
}

void Lcd1602_Write_Cmd(unsigned char cmd)     //д����
{
    Read_Busy();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_EN = 1;
    LCD1602_EN = 0;    
}

void Lcd1602_Write_Data(unsigned char dat)   //д����
{
      Read_Busy();
      LCD1602_RS = 1;
      LCD1602_RW = 0;
      LCD1602_DB = dat;
      LCD1602_EN = 1;
      LCD1602_EN = 0;
}

void LcdSetCursor(unsigned char x,unsigned char y)  //������ʾ
{
    unsigned char addr;
    if(y == 0)
        addr = 0x00 + x;
    else
        addr = 0x40 + x;
    
    Lcd1602_Write_Cmd(addr|0x80);
}

//��ָ��λ����ʾһ���ַ�
void DisplayOneChar(unsigned char X, unsigned char Y, unsigned char DData)
{
	Y &= 0x1;
	X &= 0xF; //����X���ܴ���15��Y���ܴ���1
	if (Y) X |= 0x40; //��Ҫ��ʾ�ڶ���ʱ��ַ��+0x40;
	X |= 0x80; //���ָ����
	Lcd1602_Write_Cmd(X); //��������
	Lcd1602_Write_Data(DData); //������
}


void LcdShowStr(unsigned char x,unsigned char y,unsigned char *str)     //��ʾ�ַ���
{
    LcdSetCursor(x,y);      //��ǰ�ַ�������
    while(*str != '\0')
    {
        Lcd1602_Write_Data(*str++);
    }
}

void InitLcd1602()              //1602��ʼ��
{
    Lcd1602_Write_Cmd(0x38);    //�򿪣�5*8,8λ����
    Lcd1602_Write_Cmd(0x0c);
    Lcd1602_Write_Cmd(0x06);
    Lcd1602_Write_Cmd(0x01);    //����   
}
