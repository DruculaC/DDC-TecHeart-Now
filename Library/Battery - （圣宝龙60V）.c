/*---------------------------------------------------
	Battery.c (v1.00)
	Battery201603£®60km48V£©r1
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
tByte Check2_stage = 0;
tByte Battery_level = 0;

/*----------------------------------------------------
	Check_motor_accumulator() - 48V µÁ∆Ω
	Check P20's voltage.
-----------------------------------------------------*/
void Check_motor_accumulator(void)
	{
	// detect the battery voltage
	ADC_check_result = GetADCResult(6);
	}
/*-----------------------------------------
	Broadcast_battery()
	
	??®®?¶Ã?®¢??¶Ã®∫°¿°Í?®®?1?D?¶Ã?¶Ã?®¢??¶ÃD?®Æ®≤?-?®®¶Ã??¶Ã°Í??®∞??D?¶Ã?®¢??¶Ã°‰®Æ®Æ®≤°¡?°‰®Æ¶Ã?®¢??¶Ã°Í??®∞??®®???®®£§2£§°¿°ß?°Í°§??®∞®∫®Æ?a??¶Ã?
	2?2£§°¿°ß?°Í
------------------------------------------*/
void Broadcast_battery(void)
	{
	if((ADC_check_result < load_battery_result)||(ADC_check_result > Max_battery))
		{
		load_battery_result = ADC_check_result;
		}	
	verifybattery(load_battery_result);			
	}

/*----------------------------------------------------
	verifybattery()
	voice alarm for battery status with power module
-----------------------------------------------------*/
void verifybattery(tWord Check2)
	{	
			if(Check2 < 0x250)
			{
				voice_EN = 1;
				SC_Speech(39);  		// Èê¢Áî∏Âô∫Ê∂ìÂ∂àÂÜª
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
	
/*---------------------------------------------------
	end of file
----------------------------------------------------*/