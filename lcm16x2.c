#include "configuration.h"
unsigned int data DelayConst = 140;//延迟参数，根据处理器工作频率来设置
/*LCMDelay
功能：通过循环的方式延时
参数：int，表示要延时的毫秒数
返回：
*/ 
void LCMDelay(int ms)	
{
	unsigned int i,cnt;
	cnt = DelayConst *ms;   
	for(i=0;i<cnt;i++);  
}
/*LCMReadState
功能：查询LCM的忙标志/当前AC地址
参数：
返回：BYTE,最高bit为1表示忙，为0表示闲
*/ 
unsigned char LCMReadState(void)
{
	unsigned char state;
	LCM_E=0; //E无效    
	LCM_RS=0; //指定寄存器   
	LCM_RW=1; //读 
	LCM_E=1; 
	_nop_();//等待一个机器周期    
	_nop_();//等待一个机器周期     
	state = LCM_DB;//输出   
	LCM_E = 0;
	return state;
}
/*LCMClear
功能：清屏
参数：
返回：
*/
void LCMClear(void)
{
	LCMDelay(12);   
	LCM_E=0;
	LCM_RS=0; //选择指令寄存器   
	LCM_RW=0;//写    
	LCM_DB=0x01;
	LCM_E=1;
	_nop_();
	_nop_();
	LCM_E = 0;
	LCMDelay(12);  
}
/*LCMWriteCmd
功能：向LCM写入控制字
参数：BYTE，命令字节。写入前不判断忙表示（因为初始化过程中不能判断）
返回：
*/
void LCMWriteCmd(unsigned char cmd)
{
	LCMDelay(12);
	LCM_E=0;
	LCM_RS=0;
	LCM_RW=0;
	LCM_DB=cmd;//向cmd控制字      
	LCM_E=1;
	_nop_();
	_nop_();
	LCM_E = 0;
}
/*LCMWriteData
功能：向LCM写入数据
参数：BYTE，将要写入的数据
返回：
*/
void LCMWriteData(unsigned char dc)
{
	while(LCMReadState()&BUSYFLAG);    
	LCM_RS=1;//数字寄存器
	LCM_RW=0;//写  
	LCM_DB=dc;//写入dc  
	LCM_E=1;
	_nop_();
	_nop_();
	LCM_E = 0;
}
/*LCMInit
功能：初始化LCM
参数：DWORD，晶振频率（Hz），供计算延时常数
返回：
*/
void LCMInit(void)
{
	LCMDelay(60);//延时60ms，等待LCM复位  
	LCMWriteCmd(0x38);//功能设置：8位接口，2行，5x7字符点阵
	LCMDelay(5);//延时   
	LCMWriteCmd(0x38);//第二次  
	LCMDelay(1);//延时  
	LCMWriteCmd(0x38);//此后可以通过检测忙标志判断指令执行情况  
	while(LCMReadState()&BUSYFLAG);
	LCMWriteCmd(0x08);//关闭显示	
	while(LCMReadState()&BUSYFLAG);
	LCMWriteCmd(0x01);//清屏 
	while(LCMReadState()&BUSYFLAG);
	LCMWriteCmd(0x06);//显示地址自动增量，整体不移位  
	while(LCMReadState()&BUSYFLAG);
	LCMWriteCmd(0x0e);//开显示，开光标，不闪烁    
	while(LCMReadState()&BUSYFLAG);
}
/*LCMGotoXY
功能：移动光标到X行Y列
参数：BYTE，x表示行（0,1），y表示列（横向，取值0~0x0f）
返回：
*/
void LCMGotoXY(unsigned char x,unsigned char y) 
{
	unsigned char cmd;
	if(x==0)
	{
		cmd=0x80|y;
	}
	else
	{
		cmd=0x80|0x40|y;
	}
	LCMWriteCmd(cmd); 
	while(LCMReadState()&BUSYFLAG);
}
/*LCMDisplayChar
功能：在指定位置显示一个字符
参数：x表示行（0,1），y表示列（横向取值0~0x0f），ch表示将要显示的字符（ASCII码）
返回：
*/
void LCMDisplayChar(unsigned char x, unsigned char y,unsigned char ch)
{
	LCMGotoXY(x,y);//移动光标到x行y列   
	LCMWriteData(ch);//写入ch  
}
/*LCMDisplayString
功能：从指定的位置开始显示字符串
参数：x表示行（0,1），y表示列（横向取值0~0x0f），*str为指向将要显示的字符串的指针
返回：
*/
void LCMDisplayString(unsigned char x,unsigned char y,unsigned char *str)
{
	unsigned char ptr;
	ptr=0;
	while(*(str+ptr)!=0)
	{
		LCMDisplayChar(x,(y+ptr),*(str+ptr));
		ptr++;
	}
}
/*LCMBlink
功能：将指定位置显示的字符闪烁（同时关闭光标）
参数：x表示行（0,1），y表示列（横向取值0~0x0f），cmd=BLINK：闪烁，NOBLINK：不闪烁
返回：
*/
void LCMBlink(unsigned char x,unsigned char y,unsigned char cmd) 
{
	LCMGotoXY(x,y);
	if(cmd==BLINK)
	{
		LCMWriteCmd(0x0d);
	}
	else
	{
		LCMWriteCmd(0x0c);
	}
}