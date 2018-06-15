#include "configuration.h"
/*Delay
���ܣ�ͨ��ѭ���ķ�ʽ������ʱ
������int
���أ�
*/
void Delay(unsigned int i)
{
	while(i--);
}
/*Reset18B20
���ܣ���λ18B20������оƬ�������
������
���أ�1��ʾоƬ���ڣ�0��ʾоƬ������
*/
bit Reset18B20(void)
{
	bit x;
	DQ=1;
	Delay(96);
	DQ=0;//������������ 
	Delay(960);   
	DQ=1;//�����ͷ�����   
	Delay(96);  
	x=DQ;
	Delay(240);
	if(x==0) 
		return 1;
	else 
		return 0;
}
/*DS18B20WriteByte
���ܣ��� DS18820  д��һ���ֽ�
������
���أ�
*/	
void DS18B20WriteByte(unsigned char c)
{
	unsigned char ic;
	for(ic=0;ic<8;ic++)  
	{	
		DQ=0;//�����������ߣ���ʼдλ   
		DQ=c&0x01;//�����λ��ͨ����ѭ������8λ����д��  
		Delay(60);
		DQ=1;//�ͷ����� 
		c>>=1; //����  
	}
}
/*DS18B20ReadByte
���ܣ���18B20����һ���ֽ�
������
���أ��������ֽ�
*/
unsigned char DS18B20ReadByte(void)
{
	unsigned char c,ic;
	c=0;
	for(ic=0;ic<8;ic++)
	{
		DQ=0;
		c>>=1;//����һλ
		DQ=1;
		if(DQ) 
			c|=0x80;//��c�����λ��  
		Delay(48);
	}
	return c;
}
/*DS18B20ReadTemperature
���ܣ�ģ��DS18B20��Ⲣ����¶�ֵ�����в������̣������¶�
�������¶ȣ���������С�����ֹ��ɣ�
���أ�
*/
void DS18B20ReadTemperature(Temperature *t)
{
	unsigned char tmpl,tmph;
	Reset18B20();
	Delay(144); 
	DS18B20WriteByte(SkipROM);//�����������ߣ�ʡʱ  
	DS18B20WriteByte(StartConvert);//�¶�ת��  
	Reset18B20();//��λ  
	Delay(144);   
	DS18B20WriteByte(SkipROM);//�����������ߣ�ʡʱ  
	DS18B20WriteByte(ReadMemory);//��RAM���� 
	tmpl=DS18B20ReadByte();//���¶�  
	tmph=DS18B20ReadByte();
	t->z=(int)((tmph&0x07)<<4|(tmpl&0xf0)>>4); //�¶�ת������  
	t->x=(int)((tmpl&0x0f)*625);
}