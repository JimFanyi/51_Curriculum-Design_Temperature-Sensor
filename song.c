#include "configuration.h"
/************ %BEEPER% EDIT  BELOW ***************/

static unsigned char count=0,key=0;//����������������س���
unsigned char code SONG_TONE[]={212,212,190,212,159,169,212,212,190,212,142,159,
212,212,106,126,159,169,190,119,119,126,159,142,159,0};//��������
unsigned char code SONG_LONG[]={90,30,120,120,120,240,90,30,120,120,120,240,
90,30,120,120,120,120,240,90,30,120,120,120,240,0};//���׽���
unsigned char code CharCode[]=
	{
	 0xc0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
	 0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E
	};//����ܶ���


/*!----------------------------------------------------------------------------------------------------------------------
@fn void playsong()

@brief Play HappyBirthday.  

This function automatically takes care of the beeper and play the HappyBirthday 

Currently it only play once at a time.

Example:

playsong();


Requires:
- Definitions in SONG_TUNE & SONG_LONG are correct

Promises:
- beeper will play a song

*/
void playsong()
{
	unsigned int i=0,j,k;
			while(SONG_LONG[i]!=0||SONG_TONE[i]!=0)
			{ 
			    for(j=0;j<SONG_LONG[i]*20;j++)
				{
    					AL=~AL;
					for(k=0;k<SONG_TONE[i]/3;k++);
				}
    			LCMDelay(10);
    			i++;
			}
}

/*!----------------------------------------------------------------------------------------------------------------------
@fn void Alarm()

@brief set alarm.  

This function automatically set a Draw Marquee-LED Banner and give a beeper alarm in low frequency


Example:

Alarm();


Requires:
- Definitions of P0 are correct

Promises:
-  a Draw Marquee-LED Banner and give a beeper alarm will be given

*/
void Alarm()
{
	        AL=0; //�������� 
			P0=0xFF;         
		    P0=0xFE;
		    LCMDelay (50);  
			P0=0xFD;//��һ������11111110
			LCMDelay (50); 
			P0=0xFB;//�ڶ�������11111101 
			LCMDelay (50);  
			P0=0xF7; //����������11111011 
			LCMDelay (50);   
			P0=0xEF;//���ĸ�����11110111 
			LCMDelay (50);  
			P0=0xDF; //���������11101111 
			LCMDelay (50);   
			P0=0xBF;//����������11011111
			LCMDelay (50);    
			P0=0x7F; //���߸�����10111111 
			LCMDelay (50);  
			P0=0xFF;//�ڰ˸�����01111111
			if(key==0)//�жϿ���״̬
			{
			 count++;//����+1
			 P1=CharCode[count];//�������ʾ��������
			 key=1;//���ÿ���״̬��ֹͣ����
			 if(count>9)
			 {
			 count=0;//9�κ�����
			 }
			}
			LCMDisplayString(1,0,"!!!!Warning !!!!");	
}

/*!----------------------------------------------------------------------------------------------------------------------
@fn void Alarm0()

@brief set alarm.  

This function automatically set a Draw Marquee-LED Banner and give a beeper alarm in high frequency


Example:

Alarm();


Requires:
- Definitions of P0 are correct

Promises:
-  a Draw Marquee-LED Banner and give a beeper alarm will be given

*/
void Alarm0()
{
	     AL=0;    
		  P0=0xFF; 
		  LCMDelay (1000);   
		  AL=1;        
		  P0=0xFE;
		  LCMDelay (100);  
		  P0=0xFD;//��һ������11111110
		  LCMDelay (100); 
		  P0=0xFB;//�ڶ�������11111101 
		  LCMDelay (100);  
		  P0=0xF7; //����������11111011 
		  LCMDelay (100);   
		  P0=0xEF;//���ĸ�����11110111 
		  LCMDelay (100);  
		  P0=0xDF;  //���������11101111 
		  LCMDelay (100);   
		  P0=0xBF;//����������11011111
		  LCMDelay (100);    
		  P0=0x7F; //���߸�����10111111 
		  LCMDelay (100);  
		  P0=0xFF;//�ڰ˸�����01111111
		  key=0;//������״̬���㣬�����´μ����ж� 
}