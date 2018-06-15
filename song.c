#include "configuration.h"
/************ %BEEPER% EDIT  BELOW ***************/

static unsigned char count=0,key=0;//定义计数变量及开关常量
unsigned char code SONG_TONE[]={212,212,190,212,159,169,212,212,190,212,142,159,
212,212,106,126,159,169,190,119,119,126,159,142,159,0};//乐谱音调
unsigned char code SONG_LONG[]={90,30,120,120,120,240,90,30,120,120,120,240,
90,30,120,120,120,120,240,90,30,120,120,120,240,0};//乐谱节奏
unsigned char code CharCode[]=
	{
	 0xc0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
	 0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E
	};//数码管断码


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
	        AL=0; //蜂鸣器响 
			P0=0xFF;         
		    P0=0xFE;
		    LCMDelay (50);  
			P0=0xFD;//第一个灯亮11111110
			LCMDelay (50); 
			P0=0xFB;//第二个灯亮11111101 
			LCMDelay (50);  
			P0=0xF7; //第三个灯亮11111011 
			LCMDelay (50);   
			P0=0xEF;//第四个灯亮11110111 
			LCMDelay (50);  
			P0=0xDF; //第五个灯亮11101111 
			LCMDelay (50);   
			P0=0xBF;//第六个灯亮11011111
			LCMDelay (50);    
			P0=0x7F; //第七个灯亮10111111 
			LCMDelay (50);  
			P0=0xFF;//第八个灯亮01111111
			if(key==0)//判断开关状态
			{
			 count++;//计数+1
			 P1=CharCode[count];//数码管显示技数次数
			 key=1;//重置开关状态，停止计数
			 if(count>9)
			 {
			 count=0;//9次后清零
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
		  P0=0xFD;//第一个灯亮11111110
		  LCMDelay (100); 
		  P0=0xFB;//第二个灯亮11111101 
		  LCMDelay (100);  
		  P0=0xF7; //第三个灯亮11111011 
		  LCMDelay (100);   
		  P0=0xEF;//第四个灯亮11110111 
		  LCMDelay (100);  
		  P0=0xDF;  //第五个灯亮11101111 
		  LCMDelay (100);   
		  P0=0xBF;//第六个灯亮11011111
		  LCMDelay (100);    
		  P0=0x7F; //第七个灯亮10111111 
		  LCMDelay (100);  
		  P0=0xFF;//第八个灯亮01111111
		  key=0;//将开关状态置零，便于下次计数判断 
}