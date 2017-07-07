/*---------------------------------------------------
	Battery.c (v1.00)
	Battery201603£¨60km48V£©r1
---------------------------------------------------*/

#include "main.h"
#include "port.h"

#include "Battery.h"
#include "voice.h"
#include "Delay.h"
#include "AD.h"
#define Max_battery 0x211

extern tWord ADC_check_result;		
extern tWord load_battery_result;
extern bit Battery_flag;

tByte Check2_stage = 0;
tByte Battery_level = 0;

/*----------------------------------------------------
	Check_motor_accumulator() - 48V µçÆ½
	Check P20's voltage.
-----------------------------------------------------*/
void Check_motor_accumulator(void)
	{
	// detect the battery voltage
	ADC_check_result = GetADCResult(6);
	}
/*-----------------------------------------
	Broadcast_battery()
	
	??¨¨?¦Ì?¨¢??¦Ì¨º¡À¡ê?¨¨?1?D?¦Ì?¦Ì?¨¢??¦ÌD?¨®¨²?-?¨¨¦Ì??¦Ì¡ê??¨°??D?¦Ì?¨¢??¦Ì¡ä¨®¨®¨²¡Á?¡ä¨®¦Ì?¨¢??¦Ì¡ê??¨°??¨¨???¨¨£¤2£¤¡À¡§?¡ê¡¤??¨°¨º¨®?a??¦Ì?
	2?2£¤¡À¡§?¡ê
------------------------------------------*/
void Broadcast_battery(void)
	{
	
	if(Battery_flag == 0)
	{
		if((ADC_check_result < load_battery_result)||(ADC_check_result > 0x211))
			{
			load_battery_result = ADC_check_result;
			}		
	}
	else
	{
		if((ADC_check_result < load_battery_result)||(ADC_check_result > 0x293))
			{
			load_battery_result = ADC_check_result;
			}			
	}
	
	verifybattery(load_battery_result);			
	}

/*----------------------------------------------------
	verifybattery()
	voice alarm for battery status with power module
-----------------------------------------------------*/
void verifybattery(tWord Check2)
	{	
	// µçÁ¿»¹ÄÜÐÐÊ»
	//	voice_EN = 1;
	//	SC_Speech(8);
	//	Delay(70);
	
	if(Battery_flag == 0)		// è¡¨æ˜Žä¸º48Vç”µç“¶
	{
		
		if(Check2 < 0x1e5)
		{
			voice_EN = 1;
			SC_Speech(39);  		// ç”µé‡ä¸è¶³
			Delay(30);	
		}
		else
		{
			voice_EN = 1;
			SC_Speech(8);  	
			Delay(70);		
			
			if((Check2 >= 0x1e5)&&(Check2 < 0x1ea))
				{
				SC_Speech(21);  		// 10
				Delay(30);
				//SC_Speech(16);  		// 5
				//Delay(30);
				}
			else if((Check2 >= 0x1ea)&&(Check2 < 0x1f6))
				{
				SC_Speech(13);  		// 2
				Delay(30);
				SC_Speech(21);  		// 10
				Delay(30);
				SC_Speech(12);  		// 1
				Delay(30);
				}
			else if((Check2 >= 0x1f6)&&(Check2 < 0x1fa))
				{
				SC_Speech(13);  		// 2
				Delay(30);
				SC_Speech(21);  		// 10
				Delay(30);
				SC_Speech(19);  		// 8
				Delay(30);
				}
			else if((Check2 >= 0x1fa)&&(Check2 < 0x202))
				{
				SC_Speech(14);  		// 3
				Delay(30);
				SC_Speech(21);  		// 10
				Delay(30);
				SC_Speech(15);  		// 4
				Delay(30);
				}
			else if((Check2 >= 0x202)&&(Check2 < 0x206))
				{
				SC_Speech(15);  		// 4
				Delay(30);
				SC_Speech(21);  		// 10
				Delay(30);
				SC_Speech(12);  		// 1
				Delay(30);
				}
			else if((Check2 >= 0x206)&&(Check2 < 0x20c))
				{
				SC_Speech(15);  		// 4
				Delay(30);
				SC_Speech(21);  		// 10
				Delay(30);
				SC_Speech(18);  		// 7
				Delay(30);
				}
			else if((Check2 >= 0x20c)&&(Check2 < 0x211))
				{
				SC_Speech(16);  		// 5
				Delay(30);
				SC_Speech(21);  		// 10
				Delay(30);
				SC_Speech(15);  		// 4
				Delay(30);
				}
			else if(Check2 >= 0x211)
				{
				SC_Speech(17);  		// 6
				Delay(30);
				SC_Speech(21);  		// 10
				Delay(30);
				}		
			
			SC_Speech(11);  	
			Delay(60);	
			voice_EN = 0;
			SC_RST = 1;
			Delay_50ms();
			SC_RST = 0;
			Delay_50ms();
			}		
		}
		else		// 60V ç”µç“¶
		{
			
			if(Check2 < 0x250)
			{
				voice_EN = 1;
				SC_Speech(39);  		// ç”µé‡ä¸è¶³
				Delay(30);	
			}
			else
			{
			
				voice_EN = 1;
				SC_Speech(8);  	
				Delay(70);		
								
				if((Check2 >= 0x250)&&(Check2 < 0x256))
					{
					SC_Speech(21);  		// 10
					Delay(30);
					}
				else if((Check2 >= 0x256)&&(Check2 < 0x25b))
					{
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(13);  		// 2
					Delay(30);
					}
				else if((Check2 >= 0x25b)&&(Check2 < 0x260))
					{
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(15);  		// 4
					Delay(30);
					}
				else if((Check2 >= 0x260)&&(Check2 < 0x265))
					{
					SC_Speech(13);  		// 2
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					}
				else if((Check2 >= 0x265)&&(Check2 < 0x26a))
					{
					SC_Speech(13);  		// 2
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(17);  		// 6
					Delay(30);
					}
				else if((Check2 >= 0x26a)&&(Check2 < 0x26f))
					{
					SC_Speech(14);  		// 3
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(13);  		// 2
					Delay(30);
					}
				else if((Check2 >= 0x26f)&&(Check2 < 0x274))
					{
					SC_Speech(14);  		// 3
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(19);  		// 8
					Delay(30);
					}
				else if((Check2 >= 0x274)&&(Check2 < 0x27f))
					{
					SC_Speech(15);  		// 4
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(15);  		// 4
					Delay(30);
					}
				else if((Check2 >= 0x27f)&&(Check2 < 0x284))
					{
					SC_Speech(16);  		// 5
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					}
				else if((Check2 >= 0x284)&&(Check2 < 0x289))
					{
					SC_Speech(16);  		// 5
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(17);  		// 6
					Delay(30);
					}
				else if((Check2 >= 0x289)&&(Check2 < 0x28e))
					{
					SC_Speech(17);  		// 6
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(13);  		// 2
					Delay(30);
					}
				else if((Check2 >= 0x28e)&&(Check2 < 0x293))
					{
					SC_Speech(17);  		// 6
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(19);  		// 8
					Delay(30);
					}
				else if((Check2 >= 0x293)&&(Check2 < 0x299))
					{
					SC_Speech(18);  		// 7
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(15);  		// 4
					Delay(30);
					}
				else if(Check2 >= 0x299)
				{
					SC_Speech(19);  		// 8
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
				}					
				SC_Speech(11);  	
				Delay(60);	
				voice_EN = 0;
				SC_RST = 1;
				Delay_50ms();
				SC_RST = 0;
				Delay_50ms();
			}		
		}
	}
	
/*---------------------------------------------------
	end of file
----------------------------------------------------*/