

#include "reg52.h"			 //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include "SoundPlay.h"
#include"music.h"

typedef unsigned int u16;	  //���������ͽ�����������
typedef unsigned char u8;



void Delay1ms(unsigned int count)
{
	unsigned int i,j;
	for(i=0;i<count;i++)
	for(j=0;j<120;j++);
}

void main()
{
	InitialSound();
	while(1)
	{
		Play(Music_Girl,0,3,360);
		
		Delay1ms(500);
		Play(Music_Same,0,3,360);
		Delay1ms(500);
		Play(Music_Two,0,3,360);
		Delay1ms(500);
	}
}