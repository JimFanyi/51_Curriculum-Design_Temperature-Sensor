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
	TMOD = 0x21;//选择方式2 作为波特率发生器  
	SCON = 0x50;//串口方式1，	REN=1  
    PCON |=0x80;//SMOD=1 
	TL1 = 256-(OSC/12/16/BAUDRATE);//设置定时器初值 
	TH1 = 256-(OSC/12/16/BAUDRATE);
	TR1 = 1;//启动定时器
	TI = 1;//TI有效
	//检测DS18B20温度传感器是否存在并复位传感器  
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
	
	while(1)//主循环  
	{
		Reset18B20();//复位传感器  
		Delay(144); 
		DS18B20WriteByte(SkipROM);//跳读，单总线，省时 
		DS18B20WriteByte(StartConvert); //温度转换 
		for(i=0;i<40000;i++);//等待   
		Reset18B20();//复位  
		Delay(144);   
		DS18B20WriteByte(SkipROM);//跳读，单总线，省时   
		DS18B20WriteByte(ReadMemory);//读RAM数据  
		tmp1=DS18B20ReadByte();
		tmph=DS18B20ReadByte();
		printf("\n\rTempersture code HI=%02bX,L0=%02bX",tmph,tmp1);
		DS18B20ReadTemperature(&t);//读取温度  
		printf("\nTemperature = %d.%04d",t.z,t.x); //显示当前温度 
		str[0]='N';
		str[1]='o';
		str[2]='w';
		str[3]=' ';
		str[4]='t';
		str[5]='e';
		str[6]='m';
		str[7]='p';
		str[8]=':';
		str[9]=t.z/10+'0';//十位  
		str[10]=t.z%10+'0';//个位   
		str[11]='.';
		str[12]=t.x/1000+'0';//十分位 
		str[13]=(t.x/100)%10+'0';//百分位    
		str[14]=(t.x/10)%10+'0';//千分位
		str[15]='C';         
		EA=0;//  停止所有中断    
		Ticks=0;
		ucTH=(65536-OSC/12/20)/256;//  计算 50ms 定时的时间常数  
		ucTL=(65536-OSC/12/20)%256;
		TMOD=0x21;    
		TH0=ucTH;
		TL0=ucTL;
		ET0=1;// T0 允许中断    
		TR0=1;//  启动定时器    
		EA=1;//  打开总中断允许    
		LCMInit(); //初始化LCM显示器  
		LCMClear();//清屏    
		LCMDisplayString(1,0,"Alert temp:30.00");   
		LCMDisplayString(0,0,str);//输出温度数据到显示屏  
		LCMBlink(0,15,BLINK);
		if(t.z>=30)//设置警戒线 
		{								 
			Alarm();
		}
		if(t.z>=31)//温度超过31摄氏度时，蜂鸣器响起生日歌
		{
		    playsong();
		}
		if(28<=t.z&&t.z<30)  
		{ 
		  Alarm0();
		}
		else
		{
			AL=1;//低于警戒值，蜂鸣器停止  
			P0=0xff;//全不亮11111111
		}	
	}
}