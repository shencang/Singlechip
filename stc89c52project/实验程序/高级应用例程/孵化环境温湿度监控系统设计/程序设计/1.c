#include<reg52.h>
#include "intrins.h"
typedef unsigned char uint8;
typedef unsigned int uint16;
sbit rs=P2^6;	 // ��������ѡ��
sbit rw=P2^5;	 //��дѡ��
sbit e=P2^7;	  //ʹ��

sbit k1=P3^3; //ģʽ
sbit k2=P2^1; //��
sbit k3=P2^2; //��

sbit DHT11_DQ_OUT=P3^2;
sbit led1=P3^6;
sbit led2=P3^7;

sbit dq=P2^0;

uint8 mode=0,xian;
char temph=50,templ=20;
char humih=80,humil=20;
uint8 temp,humi;
uint8 flag;	  //�趨������־
uint8 a,c,tempvalue;
uint8 code num[10]="0123456789";

uint8 code str1[]="Temp:";  //�¶�
uint8 code str2[]="Humi:";  //ʪ��
uint8 code str3[]="Error";  
uint8 code str4[]="Success    "; 
uint8 code str5[]="%RH";
uint8 code str6[]="TempH:";	  //�趨�¶�������ʾ
uint8 code str7[]="TempL:";	  //�趨�¶�������ʾ
uint8 code str8[]="HumiH:";	   //�趨ʪ��������ʾ
uint8 code str9[]="HumiL:";	   //�趨ʪ��������ʾ


void delay(uint16 i)
{
	while(i--);
}

void delay_ms(uint16 i)
{
	while(i--)
		delay(90);
}

void wrc(uint8 c)	 //д����
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
void wrd(uint8 dat)	  //д����
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
void lcd_init()	   // LCD1602��ʼ��
{
	delay(1000);
	wrc(0x38);
	wrc(0x38);	 //�����������ѡ��8λ���ߣ�˫����ʾ  5*7�����ַ�
	wrc(0x38);
	wrc(0x06);	//������ʾģʽ����  �������  �������ƶ�
	wrc(0x0c);	//��ʾ���ؿ���  ����ʾ  �޹�� ��겻��˸
	wrc(0x01);	//����ָ��  �̶���
}
//��λDHT11
void DHT11_Rst()	   
{                 
    DHT11_DQ_OUT=0; 	//����DQ
    delay_ms(20);    	//��������18ms
    DHT11_DQ_OUT=1; 	//DQ=1 
	delay(3);     	//��������20~40us
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
uint8 DHT11_Check() 	   
{   
	uint8 retry=0;	 
    while (DHT11_DQ_OUT&&retry<100)//DHT11������40~50us
	{
		retry++;
		_nop_();
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_OUT&&retry<100)//DHT11���ͺ���ٴ�����40~50us
	{
		retry++;
		_nop_();
	};
	if(retry>=100)return 1;	    
	return 0;
}


//DHT11��ʼ�� 
//����0����ʼ���ɹ���1��ʧ��
uint8 DHT11_Init()
{
	DHT11_Rst();	  
	return DHT11_Check();	
}


//��DHT11��ȡһ��λ
//����ֵ��1/0
uint8 DHT11_Read_Bit(void) 			 
{
 	uint8 retry=0;
	while(DHT11_DQ_OUT&&retry<100)//�ȴ���Ϊ�͵�ƽ 12-14us ��ʼ
	{
		retry++;
		_nop_();
	}
	retry=0;
	while((!DHT11_DQ_OUT)&&retry<100)//�ȴ���ߵ�ƽ	 26-28us��ʾ0,116-118us��ʾ1
	{
		retry++;
		_nop_();
	}
	delay(1);//�ȴ�40us
	if(DHT11_DQ_OUT)return 1;
	else return 0;		   
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
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

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
uint8 DHT11_Read_Data(uint8 *temp,uint8 *humi)    
{        
 	uint8 buf[5];
	uint8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
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




void ds18b20init()		//18b20�ĳ�ʼ��
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
void ds18b20wr(uint8 dat)	  //18b20д����
{
	uint8 i;
	for(i=0;i<8;i++)
	{
		dq=0;
		dq=dat&0x01;
		dat>>=1;
		delay(8);//��ʱ����ֻ����һ���ʱ��Ҫ����׼ȷ������ʱ��������15us
		dq=1;
		delay(1);
	}	
}
uint8 ds18b20rd()		  //18b20������
{
	uint8 value,i;
	for(i=0;i<8;i++)	
	{
		dq=0;
		value>>=1;
		dq=1;
		if(dq==1)value|=0x80;
		delay(8);//����һ��Ҳ��ʱ��Ҫ���ر�׼ȷ�����γ���������60us		
	}
	return value;
}
uint8 readtemp()			  //��ȡ�¶�����Ҫ��λ��
{
	uint8 b;
	ds18b20init();		//��ʼ��
	ds18b20wr(0xcc);   //���ͺ���ROMָ��
	ds18b20wr(0x44);   //�����¶�ת��ָ��
	delay(100);
	ds18b20init();	   //��ʼ��
	ds18b20wr(0xcc);   //���ͺ���ROMָ��
	ds18b20wr(0xbe);   //�����ݴ���ָ��
	a=ds18b20rd();	 //�¶ȵĵͰ�λ
	b=ds18b20rd();	 //�¶ȵĸ߰�λ
	b<<=4;			 //ssss s***��sΪ��־λs=0��ʾ�¶�ֵΪ������s=1�¶�ֵΪ����
	c=b&0x80;		//�¶�������־λȷ��
	b+=(a&0xf0)>>4;
	a=a&0x0f;	  //�¶ȵ�С������
	return b;
}


void key_pros()  //����������
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
	if(mode==1)		  //���¶������趨
	{
		if(k2==0)		  //��
		{
			delay(1000);
			if(k2==0)
			{
				temph++;
				if(temph>=80)temph=80;
			}
			while(!k2);
		}
		if(k3==0)	   //��
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
	if(mode==2)		  //���¶������趨
	{
		if(k2==0)		  //��
		{
			delay(1000);
			if(k2==0)
			{
				templ++;
				if(templ>=80)templ=80;
			}
			while(!k2);
		}
		if(k3==0)	   //��
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
	if(mode==3)		  //��ʪ�������趨
	{
		if(k2==0)		  //��
		{
			delay(1000);
			if(k2==0)
			{
				humih++;
				if(humih>=80)humih=80;
			}
			while(!k2);
		}
		if(k3==0)	   //��
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
	if(mode==4)		  //��ʪ�������趨
	{
		if(k2==0)		  //��
		{
			delay(1000);
			if(k2==0)
			{
				humil++;
				if(humil>=80)humil=80;
			}
			while(!k2);
		}
		if(k3==0)	   //��
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

void lcd_init_display()	   //LCD��ʼ����ʾ
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

void data_pros()	//���ݴ�����
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
	k=dio*10000;//ȡС�������λ��Ч����

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
	if(mode==1)			  //�¶�������ʾ
	{
		wrc(0x80);
		for(i=0;i<6;i++)
		{
			wrd(str6[i]);		  
		}
		wrd(temphbuf[0]);
		wrd(temphbuf[1]);			
	}
	if(mode==2)			  //�¶�������ʾ
	{
		wrc(0x80);
		for(i=0;i<6;i++)
		{
			wrd(str7[i]);		  
		}
		wrd(templbuf[0]);
		wrd(templbuf[1]);			
	}
	if(mode==3)			  //ʪ��������ʾ
	{
		wrc(0x80);
		for(i=0;i<6;i++)
		{
			wrd(str8[i]);		  
		}
		wrd(humihbuf[0]);
		wrd(humihbuf[1]);			
	}
	if(mode==4)			  //ʪ��������ʾ
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

void baojinpros()	//��������
{
	if(tempvalue>=temph||humi>=humih)	 //����¶Ȼ���ʪ�ȸ����趨����ֵ ����ʪ
	{
		led1=1;		//����ʪָʾ��
		led2=0;
	}
	if(tempvalue<=templ||humi<=humil)	//����¶Ȼ���ʪ�ȵ����趨����ֵ  ����ʪ
	{
		led1=0;
		led2=1;	   //������ʪָʾ��
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
	while(DHT11_Init())	//���DHT11�Ƿ���
	{
		for(i=0;i<5;i++)
		{
			wrc(0x80+i);
			wrd(str3[i]);	
		}			
	}
	wrc(0x01);
	lcd_init_display();	   //LCD��ʼ����ʾ
	i=0;
	while(1)
	{	
		i++;
		key_pros();
		baojinpros();	//��������
		if(i==15)
		{
			i=0;
			data_pros();  	 //��ȡһ��DHT11��������Ҫ����100ms
		}
		delay(1000); 
		
	}	
}
