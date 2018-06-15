#include "configuration.h"



/***********************************************************************************************************************
Global variable definitions with scope across entire project.
***********************************************************************************************************************/
/*--------------------------------------------------------------------------------------------------------------------*/
/* New variables (all shall start */
unsigned char ucTH,ucTL,Ticks;

/*--------------------------------------------------------------------------------------------------------------------*/
/* External global variables defined in other files (must indicate which file they are defined in) */                /*!< @brief From main.c */


  




void configuration()
{
	TMOD = 0x21;//ѡ��ʽ2 ��Ϊ�����ʷ�����  
	SCON = 0x50;//���ڷ�ʽ1��	REN=1  
    PCON |=0x80;//SMOD=1 
	TL1 = 256-(OSC/12/16/BAUDRATE);//���ö�ʱ����ֵ 
	TH1 = 256-(OSC/12/16/BAUDRATE);
	TR1 = 1;//������ʱ��
	TI = 1;//TI��Ч
	//���DS18B20�¶ȴ������Ƿ���ڲ���λ������  
	if(Reset18B20())
		printf("\r\n18B20 Reset OK!");
	else 
		printf("\r\n18B20 Not present!");
	LCM_BLC=!LCM_BLC;
}


main()
{
	unsigned char str[15];    
	unsigned int i;
	unsigned char tmph,tmp1;
	Temperature t;
	configuration();
	
	while(1)//��ѭ��  
	{
		Reset18B20();//��λ������  
		Delay(144); 
		DS18B20WriteByte(SkipROM);//�����������ߣ�ʡʱ 
		DS18B20WriteByte(StartConvert); //�¶�ת�� 
		for(i=0;i<40000;i++);//�ȴ�   
		Reset18B20();//��λ  
		Delay(144);   
		DS18B20WriteByte(SkipROM);//�����������ߣ�ʡʱ   
		DS18B20WriteByte(ReadMemory);//��RAM����  
		tmp1=DS18B20ReadByte();
		tmph=DS18B20ReadByte();
		printf("\n\rTempersture code HI=%02bX,L0=%02bX",tmph,tmp1);
		DS18B20ReadTemperature(&t);//��ȡ�¶�  
		printf("\nTemperature = %d.%04d",t.z,t.x); //��ʾ��ǰ�¶� 
		str[0]='N';
		str[1]='o';
		str[2]='w';
		str[3]=' ';
		str[4]='t';
		str[5]='e';
		str[6]='m';
		str[7]='p';
		str[8]=':';
		str[9]=t.z/10+'0';//ʮλ  
		str[10]=t.z%10+'0';//��λ   
		str[11]='.';
		str[12]=t.x/1000+'0';//ʮ��λ 
		str[13]=(t.x/100)%10+'0';//�ٷ�λ    
		str[14]=(t.x/10)%10+'0';//ǧ��λ
		str[15]='C';         
		EA=0;//  ֹͣ�����ж�    
		Ticks=0;
		ucTH=(65536-OSC/12/20)/256;//  ���� 50ms ��ʱ��ʱ�䳣��  
		ucTL=(65536-OSC/12/20)%256;
		TMOD=0x21;    
		TH0=ucTH;
		TL0=ucTL;
		ET0=1;// T0 �����ж�    
		TR0=1;//  ������ʱ��    
		EA=1;//  �����ж�����    
		LCMInit(); //��ʼ��LCM��ʾ��  
		LCMClear();//����    
		LCMDisplayString(1,0,"Alert temp:30.00");   
		LCMDisplayString(0,0,str);//����¶����ݵ���ʾ��  
		LCMBlink(0,15,BLINK);
		if(t.z>=30)//���þ����� 
		{								 
			Alarm();
		}
		if(t.z>=31)//�¶ȳ���31���϶�ʱ���������������ո�
		{
		    playsong();
		}
		if(28<=t.z&&t.z<30)  
		{ 
		  Alarm0();
		}
		else
		{
			AL=1;//���ھ���ֵ��������ֹͣ  
			P0=0xff;//ȫ����11111111
		}	
	}
}