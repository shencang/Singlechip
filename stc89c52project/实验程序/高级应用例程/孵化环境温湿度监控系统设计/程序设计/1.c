#include<reg52.h>
#include "intrins.h"
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit rs=P2^6;	 // 数据命令选择
sbit rw=P2^5;	 //读写选择
sbit e=P2^7;	  //使能

sbit k1=P3^3; //模式
sbit k2=P2^1; //加
sbit k3=P2^2; //减

sbit DHT11_DQ_OUT=P3^2;
sbit led1=P3^6;
sbit led2=P3^7;

sbit dq=P2^0;

uint8 mode=0,xian;
char temph=50,templ=20;
char humih=80,humil=20;
uint8 temp,humi;
uint8 flag;	  //设定报警标志
uint8 a,c,tempvalue;
uint8 code num[10]="0123456789";

uint8 code str1[]="Temp:";  //温度
uint8 code str2[]="Humi:";  //湿度
uint8 code str3[]="Error";  
uint8 code str4[]="Success    "; 
uint8 code str5[]="%RH";
uint8 code str6[]="TempH:";	  //设定温度上限显示
uint8 code str7[]="TempL:";	  //设定温度下限显示
uint8 code str8[]="HumiH:";	   //设定湿度上限显示
uint8 code str9[]="HumiL:";	   //设定湿度下限显示


void delay(uint16 i)
{
	while(i--);
}

void delay_ms(uint16 i)
{
	while(i--)
		delay(90);
}

void wrc(uint8 c)	 //写命令
{
	delay(1000);
	rs=0;
	rw=0;
	e=0;
	P0=c;
	e=1;
	delay(10);
	e=0;
}
void wrd(uint8 dat)	  //写数据
{
	delay(1000);
	rs=1;
	rw=0;
	e=0;
	P0=dat;
	e=1;
	delay(10);
	e=0;
	rs=0;
}
void lcd_init()	   // LCD1602初始化
{
	delay(1000);
	wrc(0x38);
	wrc(0x38);	 //功能设置命令，选择8位总线，双行显示  5*7点阵字符
	wrc(0x38);
	wrc(0x06);	//光标和显示模式设置  光标右移  整屏不移动
	wrc(0x0c);	//显示开关控制  开显示  无光标 光标不闪烁
	wrc(0x01);	//清零指令  固定的
}
//复位DHT11
void DHT11_Rst()	   
{                 
    DHT11_DQ_OUT=0; 	//拉低DQ
    delay_ms(20);    	//拉低至少18ms
    DHT11_DQ_OUT=1; 	//DQ=1 
	delay(3);     	//主机拉高20~40us
}

//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
uint8 DHT11_Check() 	   
{   
	uint8 retry=0;	 
    while (DHT11_DQ_OUT&&retry<100)//DHT11会拉低40~50us
	{
		retry++;
		_nop_();
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_OUT&&retry<100)//DHT11拉低后会再次拉高40~50us
	{
		retry++;
		_nop_();
	};
	if(retry>=100)return 1;	    
	return 0;
}


//DHT11初始化 
//返回0：初始化成功，1：失败
uint8 DHT11_Init()
{
	DHT11_Rst();	  
	return DHT11_Check();	
}


//从DHT11读取一个位
//返回值：1/0
uint8 DHT11_Read_Bit(void) 			 
{
 	uint8 retry=0;
	while(DHT11_DQ_OUT&&retry<100)//等待变为低电平 12-14us 开始
	{
		retry++;
		_nop_();
	}
	retry=0;
	while((!DHT11_DQ_OUT)&&retry<100)//等待变高电平	 26-28us表示0,116-118us表示1
	{
		retry++;
		_nop_();
	}
	delay(1);//等待40us
	if(DHT11_DQ_OUT)return 1;
	else return 0;		   
}

//从DHT11读取一个字节
//返回值：读到的数据
uint8 DHT11_Read_Byte(void)    
{        
    uint8 i,dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
uint8 DHT11_Read_Data(uint8 *temp,uint8 *humi)    
{        
 	uint8 buf[5];
	uint8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
		
	}else return 1;
	return 0;	    
}




void ds18b20init()		//18b20的初始化
{
	dq=1;
	delay(1);
	dq=0;
	delay(80);
	dq=1;
	delay(5);
	dq=0;
	delay(20);
	dq=1;
	delay(35);
}
void ds18b20wr(uint8 dat)	  //18b20写数据
{
	uint8 i;
	for(i=0;i<8;i++)
	{
		dq=0;
		dq=dat&0x01;
		dat>>=1;
		delay(8);//在时序上只有这一块对时序要求最准确，他的时间必须大于15us
		dq=1;
		delay(1);
	}	
}
uint8 ds18b20rd()		  //18b20读数据
{
	uint8 value,i;
	for(i=0;i<8;i++)	
	{
		dq=0;
		value>>=1;
		dq=1;
		if(dq==1)value|=0x80;
		delay(8);//在这一块也对时间要求特别准确，整段程序必须大于60us		
	}
	return value;
}
uint8 readtemp()			  //读取温度内需要复位的
{
	uint8 b;
	ds18b20init();		//初始化
	ds18b20wr(0xcc);   //发送忽略ROM指令
	ds18b20wr(0x44);   //发送温度转换指令
	delay(100);
	ds18b20init();	   //初始化
	ds18b20wr(0xcc);   //发送忽略ROM指令
	ds18b20wr(0xbe);   //发读暂存器指令
	a=ds18b20rd();	 //温度的低八位
	b=ds18b20rd();	 //温度的高八位
	b<<=4;			 //ssss s***；s为标志位s=0表示温度值为正数，s=1温度值为负数
	c=b&0x80;		//温度正负标志位确认
	b+=(a&0xf0)>>4;
	a=a&0x0f;	  //温度的小数部分
	return b;
}


void key_pros()  //按键处理函数
{
	if(k1==0)
	{
		delay(1000);
		if(k1==0)
		{
			mode++;
			if(mode==5)mode=0;
			wrc(0x01);
		}
		while(!k1);
	}
	if(mode==1)		  //对温度上限设定
	{
		if(k2==0)		  //加
		{
			delay(1000);
			if(k2==0)
			{
				temph++;
				if(temph>=80)temph=80;
			}
			while(!k2);
		}
		if(k3==0)	   //减
		{
			delay(1000);
			if(k3==0)
			{
				temph--;
				if(temph<=0)temph=0;
			}
			while(!k3);
		}
	}
	if(mode==2)		  //对温度下限设定
	{
		if(k2==0)		  //加
		{
			delay(1000);
			if(k2==0)
			{
				templ++;
				if(templ>=80)templ=80;
			}
			while(!k2);
		}
		if(k3==0)	   //减
		{
			delay(1000);
			if(k3==0)
			{
				templ--;
				if(templ<=0)templ=0;
			}
			while(!k3);
		}
	}
	if(mode==3)		  //对湿度上限设定
	{
		if(k2==0)		  //加
		{
			delay(1000);
			if(k2==0)
			{
				humih++;
				if(humih>=80)humih=80;
			}
			while(!k2);
		}
		if(k3==0)	   //减
		{
			delay(1000);
			if(k3==0)
			{
				humih--;
				if(humih<=0)humih=0;
			}
			while(!k3);
		}
	}
	if(mode==4)		  //对湿度下限设定
	{
		if(k2==0)		  //加
		{
			delay(1000);
			if(k2==0)
			{
				humil++;
				if(humil>=80)humil=80;
			}
			while(!k2);
		}
		if(k3==0)	   //减
		{
			delay(1000);
			if(k3==0)
			{
				humil--;
				if(humil<=0)humil=0;
			}
			while(!k3);
		}
	}
}

void lcd_init_display()	   //LCD初始化显示
{
	uint8 i;
	for(i=0;i<5;i++)
	{
		wrc(0x80+i);
		wrd(str1[i]);	
	}

	for(i=0;i<5;i++)
	{
		wrc(0xc0+i);
		wrd(str2[i]);	
	}
}

void data_pros()	//数据处理函数
{
	uint8 i;  	    
	uint8 temp_buf[2],humi_buf[2];
	uint8 temphbuf[2],templbuf[2],humihbuf[2],humilbuf[2];
	float dio;
	uint16 k;

	tempvalue=readtemp();
	DHT11_Read_Data(&temp,&humi);
	temp_buf[0]=temp/10+0x30;	
	temp_buf[1]=temp%10+0x30;

	humi_buf[0]=humi/10+0x30;	
	humi_buf[1]=humi%10+0x30;
	
	dio=a*0.0625;
	k=dio*10000;//取小数点后两位有效数字

	temphbuf[0]=temph/10+0x30;
	temphbuf[1]=temph%10+0x30;
	templbuf[0]=templ/10+0x30;
	templbuf[1]=templ%10+0x30;

	humihbuf[0]=humih/10+0x30;
	humihbuf[1]=humih%10+0x30;
	humilbuf[0]=humil/10+0x30;
	humilbuf[1]=humil%10+0x30;

	if(mode==0)
	{
		lcd_init_display();
		wrc(0x85);
		wrd(num[tempvalue%100/10]);
		wrd(num[tempvalue%100%10]);
		wrd('.');
		wrd(num[k/1000]);
		wrd(0xdf);
		wrd('C');
	
		for(i=0;i<2;i++)
		{
			wrc(0Xc5+i);
			wrd(humi_buf[i]);		  
		}	
		for(i=0;i<3;i++)
		{
			wrc(0Xc7+i);
			wrd(str5[i]);		  
		}	
	}
	if(mode==1)			  //温度上限显示
	{
		wrc(0x80);
		for(i=0;i<6;i++)
		{
			wrd(str6[i]);		  
		}
		wrd(temphbuf[0]);
		wrd(temphbuf[1]);			
	}
	if(mode==2)			  //温度下限显示
	{
		wrc(0x80);
		for(i=0;i<6;i++)
		{
			wrd(str7[i]);		  
		}
		wrd(templbuf[0]);
		wrd(templbuf[1]);			
	}
	if(mode==3)			  //湿度上限显示
	{
		wrc(0x80);
		for(i=0;i<6;i++)
		{
			wrd(str8[i]);		  
		}
		wrd(humihbuf[0]);
		wrd(humihbuf[1]);			
	}
	if(mode==4)			  //湿度下限显示
	{
		wrc(0x80);
		for(i=0;i<6;i++)
		{
			wrd(str9[i]);		  
		}
		wrd(humilbuf[0]);
		wrd(humilbuf[1]);			
	}
}

void baojinpros()	//报警处理
{
	if(tempvalue>=temph||humi>=humih)	 //检测温度或者湿度高于设定上限值 降温湿
	{
		led1=1;		//降温湿指示灯
		led2=0;
	}
	if(tempvalue<=templ||humi<=humil)	//检测温度或者湿度低于设定下限值  升温湿
	{
		led1=0;
		led2=1;	   //升高温湿指示灯
	}
	if((tempvalue>templ&&tempvalue<temph)&&(humi>humil&&humi<humih))
	{
		led1=0;
		led2=0;	
	}
}

void main()
{
	uint8 i=0;
	led1=0;
	led2=0;
	lcd_init();
	while(DHT11_Init())	//检测DHT11是否纯在
	{
		for(i=0;i<5;i++)
		{
			wrc(0x80+i);
			wrd(str3[i]);	
		}			
	}
	wrc(0x01);
	lcd_init_display();	   //LCD初始化显示
	i=0;
	while(1)
	{	
		i++;
		key_pros();
		baojinpros();	//报警处理
		if(i==15)
		{
			i=0;
			data_pros();  	 //读取一次DHT11数据最少要大于100ms
		}
		delay(1000); 
		
	}	
}
