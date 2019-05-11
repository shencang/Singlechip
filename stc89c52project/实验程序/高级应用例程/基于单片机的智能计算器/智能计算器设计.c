/*  S16 S12 S8 S4分别是123+
	S15 S11 S7 S3分别是456-
	S14 S10 S6 S2分别是789*
	S13 S9 S5 S1分别是0 CLR = /
*/

/* 实现两个数的运算，每个数的位数至少可以八位  */
#include<reg52.h>			
typedef unsigned char uint8;
typedef unsigned int uint16;

sbit rw=P2^5;
sbit rs=P2^6;
sbit e=P2^7;
sbit led=P3^7;

sbit beep=P2^0;
uint8 key,num;
uint8 fuhao;//定义具体的那个符号，是加减还是乘除。
uint8 flag;	//定义有没有按下符号键，这个是统称
long a,b,c,d;	//定义运算数据的第一个和第二个及等于的数变量
uint8 k; //定义小数点后面显示的位数
uint8 biao;

uint8 dat1[]={1,2,3,0x2b-0x30, 4,5,6,0x2d-0x30, 7,8,9,0x2a-0x30, 0,0x01-0x30,0x3d-0x30,0x2b-0x30 };//保存显示的数据

void delay(uint16 i)
{
	while(i--);
}
void lcdwrc(uint8 c)
{
	delay(1000);
	rs=0;
	rw=0;
	e=0;
	P0=c;
	e=1;
	delay(1000);
	e=0;
}
void lcdwrd(uint8 dat)
{
	delay(1000);
	rs=1;
	rw=0;
	e=0;
	P0=dat;
	e=1;
	delay(1000);
	e=0;
	rs=0;
}

void lcdinit()
{
	delay(1500);
	lcdwrc(0x38);
	delay(500);
	lcdwrc(0x38);
	delay(500);
	lcdwrc(0x38);
	delay(500);
	lcdwrc(0x38);
	lcdwrc(0x08);
	lcdwrc(0x01);
	lcdwrc(0x06);
	lcdwrc(0x0c);
	key=0;
	num=0;
	flag=0;
	fuhao=0;
	a=0;
	b=0;
	c=0;
	d=0;
	biao=0;
	led=0;

}

void keyscan()
{
	P1=0xfe;   //令第一行为0，然后判断是哪一列按下
	if(P1!=0xfe)
	{
		delay(1000);
		if(P1!=0xfe)
		{
			key=P1&0xf0;
			switch(key)
			{
				case 0xe0: num=0;break;	  //1
				case 0xd0: num=1;break;	  //2
				case 0xb0: num=2;break;	  //3
				case 0x70: num=3;break;	  //加
			}
		}
		while(P1!=0xfe);
		if(num==0||num==1||num==2)	 //确认第一行的数1,2,3
		{
			if(flag==0)	 //没有按下符号键
			{
				//led=1;
				a=a*10+dat1[num];	
			}
			else
			{
				//led=1;
				b=b*10+dat1[num];
			}
		
		}
		if(num==3)
		{
			//led=0;
			flag=1;
			fuhao=1;//加号+	
		}
		lcdwrd(0x30+dat1[num]);
	}


	P1=0xfd;				//令第二行为0，判断是哪一列按下
	if(P1!=0xfd)
	{
		delay(1000);
		if(P1!=0xfd)
		{
			key=P1&0xf0;
			switch(key)
			{
				case 0xe0: num=4;break;	  //4
				case 0xd0: num=5;break;	  //5
				case 0xb0: num=6;break;	  //6
				case 0x70: num=7;break;	  //减—
			}	
		}
		while(P1!=0xfd);
		if(num==4||num==5||num==6)
		{
			if(flag==0)	 //没有按下符号键
			{
				//led=1;
				a=a*10+dat1[num];	
			}
			else
			{
				//led=1;
				b=b*10+dat1[num];
			}			
		}
		else
		{
			flag=1;
			fuhao=2;//带表减号
		}
		lcdwrd(0x30+dat1[num]);
	}


	P1=0xfb;		 //令第三行为0，判断哪一列按下
	if(P1!=0xfb)
	{
		delay(1000);
		if(P1!=0xfb)
		{
			key=P1&0xf0;
			switch(key)
			{
				case 0xe0: num=8;break;	  //7
				case 0xd0: num=9;break;	  //8
				case 0xb0: num=10;break;  //9
				case 0x70: num=11;break;  //乘*
			}	
		}
		while(P1!=0xfb);
		if(num==8||num==9||num==10)
		{
			if(flag==0)	 //没有按下符号键
			{
				//led=1;
				a=a*10+dat1[num];	
			}
			else
			{
				//led=1;
				b=b*10+dat1[num];
			}			
		}
		else
		{
			flag=1;
			fuhao=3;//带表乘号*
		}
		lcdwrd(0x30+dat1[num]);
	}


	P1=0xf7;		 //令第四行为0，判断哪一列按下
	if(P1!=0xf7)
	{
		delay(1000);
		if(P1!=0xf7)
		{
			key=P1&0xf0;
			switch(key)
			{
				case 0xe0: num=12;break;  //0
				case 0xd0: num=13;break;  //清除rst
				case 0xb0: num=14;break;  //等号=
				case 0x70: num=15;break;  //除/
			}	
		}
		while(P1!=0xf7);
		switch(num)
		{
			case 12: 
					if(flag==0)	 //没有按下符号键
					{
						//led=1;
						a=a*10+dat1[num];
						lcdwrd(0x30);	
					}
					else
					{
						//led=1;
						b=b*10+dat1[num];
						lcdwrd(0x30);
					}
					break;
			
			case 13: 
					lcdwrc(0x01);	//清屏指令			
					a=0;
					b=0;
					flag=0;
					fuhao=0;
					break;

			case 15:
			
					flag=1;
					fuhao=4;
					lcdwrd(0x2f);//除号/
					break;
					
			case 14: 
					if(fuhao==1)//加
					{
						lcdwrc(0x4f+0x80);
						lcdwrc(0x04);//设置光标左移，屏幕不移动
						c=a+b;
						while(c!=0)	 //一位一位显示
						{
							lcdwrd(0x30+c%10);//显示结果的最后一位在0x4f的位置
							c=c/10;//取前面的结果数据	
						}
						lcdwrd(0x3d); //显示等于号=
						a=0;
						b=0;
						flag=0;
						fuhao=0;//全部清除为0
					}
					if(fuhao==2)   //减
					{
						lcdwrc(0x4f+0x80);
						lcdwrc(0x04);//设置光标左移，屏幕不移动
						if(a>b)
							c=a-b;
						else
							c=b-a;
						
						while(c!=0)	 //一位一位显示
						{
							lcdwrd(0x30+c%10);//显示结果的最后一位在0x4f的位置
							c=c/10;//取前面的结果数据	
						}
						if(a<b)	lcdwrd(0x2d); //显示-号
						lcdwrd(0x3d); //显示等于号=
						a=0;
						b=0;
						flag=0;
						fuhao=0;//全部清除为0
					}
					if(fuhao==3)//乘法		
					{
						lcdwrc(0x4f+0x80);
						lcdwrc(0x04);//设置光标左移，屏幕不移动
						c=a*b;
						while(c!=0)	 //一位一位显示
						{
							lcdwrd(0x30+c%10);//显示结果的最后一位在0x4f的位置
							c=c/10;//取前面的结果数据	
						}
						lcdwrd(0x3d); //显示等于号=
						a=0;
						b=0;
						flag=0;
						fuhao=0;//全部清除为0	
					}
					if(fuhao==3)//乘法		
					{
						lcdwrc(0x4f+0x80);
						lcdwrc(0x04);//设置光标左移，屏幕不移动
						c=a*b;
						while(c!=0)	 //一位一位显示
						{
							lcdwrd(0x30+c%10);//显示结果的最后一位在0x4f的位置
							c=c/10;//取前面的结果数据	
						}
						lcdwrd(0x3d); //显示等于号=
						a=0;
						b=0;
						flag=0;
						fuhao=0;//全部清除为0	
					}
					if(fuhao==4)
					{
						k=0;
						lcdwrc(0x4f+0x80);
						lcdwrc(0x04);//设置光标左移，屏幕不移动
						c=(long)(((float)a/b)*1000);//强制转换为long。
						while(c!=0)	 //一位一位显示
						{
							k++;
							lcdwrd(0x30+c%10);//显示结果的最后一位在0x4f的位置
							c=c/10;//取前面的结果数据
							if(k==3)
							{
								lcdwrd(0x2e);
								k=0;
							}		
						}
						if(a/b<0)	  //如果a比b小的话那么除的结果最高位是0
						{
							lcdwrd(0x30);	
						}
						lcdwrd(0x3d); //显示等号
						a=0;
						b=0;
						flag=0;
						fuhao=0;//全部清除为0
					}
					break;

		}
	}

}



void main()
{
	lcdinit();
	while(1)
	{
		keyscan();	
	}
}
