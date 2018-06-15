/*!**********************************************************************************************************************
@file ds18b20.h                                                               
@brief Header file for ds18b20.c
**********************************************************************************************************************/
#ifndef _DS18B20H_ 
#define _DS18B20H_
#define ReadROM 0x33  
#define MatchROM 0x55 
#define SkipROM 0xcc  
#define SearchROM 0xf0  
#define StartConvert 0x44 
#define ReadMemory 0xbe


/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/
typedef struct
{
	int z;//整数部  
	int x;//小数部分  
} Temperature;



/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/
void Delay(unsigned int i);
bit Reset18B20(void);
void DS18B20WriteByte(unsigned char c);
unsigned char DS18B20ReadByte(void);
void DS18B20ReadTemperature(Temperature *t);


/*------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/

#endif /* _DS18B20_H */
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
