#include "configuration.h"
/*Delay
功能：通过循环的方式——延时
参数：int
返回：
*/
void Delay(unsigned int i)
{
	while(i--);
}
/*Reset18B20
功能：复位18B20并返回芯片存在与否
参数：
返回：1表示芯片存在，0表示芯片不存在
*/
bit Reset18B20(void)
{
	bit x;
	DQ=1;
	Delay(96);
	DQ=0;//主机拉低总线 
	Delay(960);   
	DQ=1;//主机释放总线   
	Delay(96);  
	x=DQ;
	Delay(240);
	if(x==0) 
		return 1;
	else 
		return 0;
}
/*DS18B20WriteByte
功能：向 DS18820  写入一个字节
参数：
返回：
*/	
void DS18B20WriteByte(unsigned char c)
{
	unsigned char ic;
	for(ic=0;ic<8;ic++)  
	{	
		DQ=0;//主机拉低总线，开始写位   
		DQ=c&0x01;//存最低位，通过右循环，将8位数据写入  
		Delay(60);
		DQ=1;//释放总线 
		c>>=1; //右移  
	}
}
/*DS18B20ReadByte
功能：从18B20读出一个字节
参数：
返回：读出的字节
*/
unsigned char DS18B20ReadByte(void)
{
	unsigned char c,ic;
	c=0;
	for(ic=0;ic<8;ic++)
	{
		DQ=0;
		c>>=1;//右移一位
		DQ=1;
		if(DQ) 
			c|=0x80;//将c的最高位置  
		Delay(48);
	}
	return c;
}
/*DS18B20ReadTemperature
功能：模拟DS18B20检测并输出温度值的所有操作过程，读出温度
参数：温度（由整数和小数部分构成）
返回：
*/
void DS18B20ReadTemperature(Temperature *t)
{
	unsigned char tmpl,tmph;
	Reset18B20();
	Delay(144); 
	DS18B20WriteByte(SkipROM);//跳读，单总线，省时  
	DS18B20WriteByte(StartConvert);//温度转换  
	Reset18B20();//复位  
	Delay(144);   
	DS18B20WriteByte(SkipROM);//跳读，单总线，省时  
	DS18B20WriteByte(ReadMemory);//读RAM数据 
	tmpl=DS18B20ReadByte();//读温度  
	tmph=DS18B20ReadByte();
	t->z=(int)((tmph&0x07)<<4|(tmpl&0xf0)>>4); //温度转换计算  
	t->x=(int)((tmpl&0x0f)*625);
}