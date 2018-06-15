#include "configuration.h"
unsigned int data DelayConst = 140;//�ӳٲ��������ݴ���������Ƶ��������
/*LCMDelay
���ܣ�ͨ��ѭ���ķ�ʽ��ʱ
������int����ʾҪ��ʱ�ĺ�����
���أ�
*/ 
void LCMDelay(int ms)	
{
	unsigned int i,cnt;
	cnt = DelayConst *ms;   
	for(i=0;i<cnt;i++);  
}
/*LCMReadState
���ܣ���ѯLCM��æ��־/��ǰAC��ַ
������
���أ�BYTE,���bitΪ1��ʾæ��Ϊ0��ʾ��
*/ 
unsigned char LCMReadState(void)
{
	unsigned char state;
	LCM_E=0; //E��Ч    
	LCM_RS=0; //ָ���Ĵ���   
	LCM_RW=1; //�� 
	LCM_E=1; 
	_nop_();//�ȴ�һ����������    
	_nop_();//�ȴ�һ����������     
	state = LCM_DB;//���   
	LCM_E = 0;
	return state;
}
/*LCMClear
���ܣ�����
������
���أ�
*/
void LCMClear(void)
{
	LCMDelay(12);   
	LCM_E=0;
	LCM_RS=0; //ѡ��ָ��Ĵ���   
	LCM_RW=0;//д    
	LCM_DB=0x01;
	LCM_E=1;
	_nop_();
	_nop_();
	LCM_E = 0;
	LCMDelay(12);  
}
/*LCMWriteCmd
���ܣ���LCMд�������
������BYTE�������ֽڡ�д��ǰ���ж�æ��ʾ����Ϊ��ʼ�������в����жϣ�
���أ�
*/
void LCMWriteCmd(unsigned char cmd)
{
	LCMDelay(12);
	LCM_E=0;
	LCM_RS=0;
	LCM_RW=0;
	LCM_DB=cmd;//��cmd������      
	LCM_E=1;
	_nop_();
	_nop_();
	LCM_E = 0;
}
/*LCMWriteData
���ܣ���LCMд������
������BYTE����Ҫд�������
���أ�
*/
void LCMWriteData(unsigned char dc)
{
	while(LCMReadState()&BUSYFLAG);    
	LCM_RS=1;//���ּĴ���
	LCM_RW=0;//д  
	LCM_DB=dc;//д��dc  
	LCM_E=1;
	_nop_();
	_nop_();
	LCM_E = 0;
}
/*LCMInit
���ܣ���ʼ��LCM
������DWORD������Ƶ�ʣ�Hz������������ʱ����
���أ�
*/
void LCMInit(void)
{
	LCMDelay(60);//��ʱ60ms���ȴ�LCM��λ  
	LCMWriteCmd(0x38);//�������ã�8λ�ӿڣ�2�У�5x7�ַ�����
	LCMDelay(5);//��ʱ   
	LCMWriteCmd(0x38);//�ڶ���  
	LCMDelay(1);//��ʱ  
	LCMWriteCmd(0x38);//�˺����ͨ�����æ��־�ж�ָ��ִ�����  
	while(LCMReadState()&BUSYFLAG);
	LCMWriteCmd(0x08);//�ر���ʾ	
	while(LCMReadState()&BUSYFLAG);
	LCMWriteCmd(0x01);//���� 
	while(LCMReadState()&BUSYFLAG);
	LCMWriteCmd(0x06);//��ʾ��ַ�Զ����������岻��λ  
	while(LCMReadState()&BUSYFLAG);
	LCMWriteCmd(0x0e);//����ʾ������꣬����˸    
	while(LCMReadState()&BUSYFLAG);
}
/*LCMGotoXY
���ܣ��ƶ���굽X��Y��
������BYTE��x��ʾ�У�0,1����y��ʾ�У�����ȡֵ0~0x0f��
���أ�
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
���ܣ���ָ��λ����ʾһ���ַ�
������x��ʾ�У�0,1����y��ʾ�У�����ȡֵ0~0x0f����ch��ʾ��Ҫ��ʾ���ַ���ASCII�룩
���أ�
*/
void LCMDisplayChar(unsigned char x, unsigned char y,unsigned char ch)
{
	LCMGotoXY(x,y);//�ƶ���굽x��y��   
	LCMWriteData(ch);//д��ch  
}
/*LCMDisplayString
���ܣ���ָ����λ�ÿ�ʼ��ʾ�ַ���
������x��ʾ�У�0,1����y��ʾ�У�����ȡֵ0~0x0f����*strΪָ��Ҫ��ʾ���ַ�����ָ��
���أ�
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
���ܣ���ָ��λ����ʾ���ַ���˸��ͬʱ�رչ�꣩
������x��ʾ�У�0,1����y��ʾ�У�����ȡֵ0~0x0f����cmd=BLINK����˸��NOBLINK������˸
���أ�
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