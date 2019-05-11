/*  S16 S12 S8 S4�ֱ���123+
	S15 S11 S7 S3�ֱ���456-
	S14 S10 S6 S2�ֱ���789*
	S13 S9 S5 S1�ֱ���0 CLR = /
*/

/* ʵ�������������㣬ÿ������λ�����ٿ��԰�λ  */
#include<reg52.h>			
typedef unsigned char uint8;
typedef unsigned int uint16;

sbit rw=P2^5;
sbit rs=P2^6;
sbit e=P2^7;
sbit led=P3^7;

sbit beep=P2^0;
uint8 key,num;
uint8 fuhao;//���������Ǹ����ţ��ǼӼ����ǳ˳���
uint8 flag;	//������û�а��·��ż��������ͳ��
long a,b,c,d;	//�����������ݵĵ�һ���͵ڶ��������ڵ�������
uint8 k; //����С���������ʾ��λ��
uint8 biao;

uint8 dat1[]={1,2,3,0x2b-0x30, 4,5,6,0x2d-0x30, 7,8,9,0x2a-0x30, 0,0x01-0x30,0x3d-0x30,0x2b-0x30 };//������ʾ������

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
	P1=0xfe;   //���һ��Ϊ0��Ȼ���ж�����һ�а���
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
				case 0x70: num=3;break;	  //��
			}
		}
		while(P1!=0xfe);
		if(num==0||num==1||num==2)	 //ȷ�ϵ�һ�е���1,2,3
		{
			if(flag==0)	 //û�а��·��ż�
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
			fuhao=1;//�Ӻ�+	
		}
		lcdwrd(0x30+dat1[num]);
	}


	P1=0xfd;				//��ڶ���Ϊ0���ж�����һ�а���
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
				case 0x70: num=7;break;	  //����
			}	
		}
		while(P1!=0xfd);
		if(num==4||num==5||num==6)
		{
			if(flag==0)	 //û�а��·��ż�
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
			fuhao=2;//�������
		}
		lcdwrd(0x30+dat1[num]);
	}


	P1=0xfb;		 //�������Ϊ0���ж���һ�а���
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
				case 0x70: num=11;break;  //��*
			}	
		}
		while(P1!=0xfb);
		if(num==8||num==9||num==10)
		{
			if(flag==0)	 //û�а��·��ż�
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
			fuhao=3;//����˺�*
		}
		lcdwrd(0x30+dat1[num]);
	}


	P1=0xf7;		 //�������Ϊ0���ж���һ�а���
	if(P1!=0xf7)
	{
		delay(1000);
		if(P1!=0xf7)
		{
			key=P1&0xf0;
			switch(key)
			{
				case 0xe0: num=12;break;  //0
				case 0xd0: num=13;break;  //���rst
				case 0xb0: num=14;break;  //�Ⱥ�=
				case 0x70: num=15;break;  //��/
			}	
		}
		while(P1!=0xf7);
		switch(num)
		{
			case 12: 
					if(flag==0)	 //û�а��·��ż�
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
					lcdwrc(0x01);	//����ָ��			
					a=0;
					b=0;
					flag=0;
					fuhao=0;
					break;

			case 15:
			
					flag=1;
					fuhao=4;
					lcdwrd(0x2f);//����/
					break;
					
			case 14: 
					if(fuhao==1)//��
					{
						lcdwrc(0x4f+0x80);
						lcdwrc(0x04);//���ù�����ƣ���Ļ���ƶ�
						c=a+b;
						while(c!=0)	 //һλһλ��ʾ
						{
							lcdwrd(0x30+c%10);//��ʾ��������һλ��0x4f��λ��
							c=c/10;//ȡǰ��Ľ������	
						}
						lcdwrd(0x3d); //��ʾ���ں�=
						a=0;
						b=0;
						flag=0;
						fuhao=0;//ȫ�����Ϊ0
					}
					if(fuhao==2)   //��
					{
						lcdwrc(0x4f+0x80);
						lcdwrc(0x04);//���ù�����ƣ���Ļ���ƶ�
						if(a>b)
							c=a-b;
						else
							c=b-a;
						
						while(c!=0)	 //һλһλ��ʾ
						{
							lcdwrd(0x30+c%10);//��ʾ��������һλ��0x4f��λ��
							c=c/10;//ȡǰ��Ľ������	
						}
						if(a<b)	lcdwrd(0x2d); //��ʾ-��
						lcdwrd(0x3d); //��ʾ���ں�=
						a=0;
						b=0;
						flag=0;
						fuhao=0;//ȫ�����Ϊ0
					}
					if(fuhao==3)//�˷�		
					{
						lcdwrc(0x4f+0x80);
						lcdwrc(0x04);//���ù�����ƣ���Ļ���ƶ�
						c=a*b;
						while(c!=0)	 //һλһλ��ʾ
						{
							lcdwrd(0x30+c%10);//��ʾ��������һλ��0x4f��λ��
							c=c/10;//ȡǰ��Ľ������	
						}
						lcdwrd(0x3d); //��ʾ���ں�=
						a=0;
						b=0;
						flag=0;
						fuhao=0;//ȫ�����Ϊ0	
					}
					if(fuhao==3)//�˷�		
					{
						lcdwrc(0x4f+0x80);
						lcdwrc(0x04);//���ù�����ƣ���Ļ���ƶ�
						c=a*b;
						while(c!=0)	 //һλһλ��ʾ
						{
							lcdwrd(0x30+c%10);//��ʾ��������һλ��0x4f��λ��
							c=c/10;//ȡǰ��Ľ������	
						}
						lcdwrd(0x3d); //��ʾ���ں�=
						a=0;
						b=0;
						flag=0;
						fuhao=0;//ȫ�����Ϊ0	
					}
					if(fuhao==4)
					{
						k=0;
						lcdwrc(0x4f+0x80);
						lcdwrc(0x04);//���ù�����ƣ���Ļ���ƶ�
						c=(long)(((float)a/b)*1000);//ǿ��ת��Ϊlong��
						while(c!=0)	 //һλһλ��ʾ
						{
							k++;
							lcdwrd(0x30+c%10);//��ʾ��������һλ��0x4f��λ��
							c=c/10;//ȡǰ��Ľ������
							if(k==3)
							{
								lcdwrd(0x2e);
								k=0;
							}		
						}
						if(a/b<0)	  //���a��bС�Ļ���ô���Ľ�����λ��0
						{
							lcdwrd(0x30);	
						}
						lcdwrd(0x3d); //��ʾ�Ⱥ�
						a=0;
						b=0;
						flag=0;
						fuhao=0;//ȫ�����Ϊ0
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
