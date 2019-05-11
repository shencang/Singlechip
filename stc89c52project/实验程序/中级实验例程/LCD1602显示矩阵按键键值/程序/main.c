/*******************************************************
*效果：烧录如果显示不正常，重新上电即可。按下矩阵键盘显示对应按键号
*
*
*
*********************************************************/

#include<reg51.h>	
#include"lcd.h"

#define GPIO_KEY P1

unsigned char KeyValue;
//用来存放读取到的键值
unsigned char KeyState;
//用来存放按键状态
unsigned char PuZh[]=" Pechin Science ";
unsigned char dat[]="0123456789ABCDEF";
void Delay10ms();   //延时50us
void KeyDown();		 //检测按键函数
 
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void main(void)
{
	unsigned char i;
	LcdInit();
	KeyState=0;
	for(i=0;i<16;i++)
	{
//		LcdWriteCom(0x80);
		LcdWriteData(PuZh[i]);	
	}
	while(1)
	{
		KeyDown();
		if(KeyState)
		{
			KeyState=0;
			LcdWriteCom(0x80+0x40);
			LcdWriteData(dat[KeyValue]);
		}
	}				
}
/*******************************************************************************
* 函 数 名         : KeyDown
* 函数功能		   : 检测有按键按下并读取键值
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KeyDown(void)
{
	char a;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f)
	{
		Delay10ms();
		if(GPIO_KEY!=0x0f)
		{
			KeyState=1;
			//测试列
			GPIO_KEY=0X0F;
// 			Delay10ms();
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue=0;break;
				case(0X0b):	KeyValue=1;break;
				case(0X0d): KeyValue=2;break;
				case(0X0e):	KeyValue=3;break;
//				default:	KeyValue=17;	//检测出错回复17意思是把数码管全灭掉。
			}
			//测试行
			GPIO_KEY=0XF0;
			Delay10ms();
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue=KeyValue;break;
				case(0Xb0):	KeyValue=KeyValue+4;break;
				case(0Xd0): KeyValue=KeyValue+8;break;
				case(0Xe0):	KeyValue=KeyValue+12;break;
//				default:	KeyValue=17;
			}
			while((a<50)&&(GPIO_KEY!=0xf0))	 //检测按键松手检测
			{
				Delay10ms();
				a++;
			}
			a=0;
		}
	}
}
/*******************************************************************************
* 函 数 名         : Delay10ms
* 函数功能		   : 延时函数，延时10ms
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Delay10ms(void)   //误差 0us
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}
