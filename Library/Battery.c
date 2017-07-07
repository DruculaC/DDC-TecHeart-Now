/*---------------------------------------------------
	Battery.c (v1.00)
	C3ÔºåÂú£ÂÆùÈæô48V60VÈÄöÁî®Á®ãÂ∫è„ÄÇ
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
	// µÁ¡øªπƒ‹–– ª
	//	voice_EN = 1;
	//	SC_Speech(8);
	//	Delay(70);
	
	if(Battery_flag == 0)		// Ë°®Êòé‰∏∫48VÁîµÁì∂
	{
		
		if(Check2 < 0x1dd)
		{
			voice_EN = 1;
			SC_Speech(39);  		// ÁîµÈáè‰∏çË∂≥
			Delay(30);	
		}
		else
		{
			voice_EN = 1;
			SC_Speech(8);  	
			Delay(70);		
			
			if((Check2 >= 0x1dd)&&(Check2 < 0x1e5))
				{
				SC_Speech(21);  		// 10
				Delay(30);
				SC_Speech(13);  		// 2
				Delay(30);
				}
			else if((Check2 >= 0x1e5)&&(Check2 < 0x1ee))
				{
				SC_Speech(13);  		// 2
				Delay(30);
				SC_Speech(21);  		// 10
				Delay(30);
				}
			else if((Check2 >= 0x1ee)&&(Check2 < 0x1f7))
				{
				SC_Speech(13);  		// 2
				Delay(30);
				SC_Speech(21);  		// 10
				Delay(30);
				SC_Speech(19);  		// 8
				Delay(30);
				}
			else if((Check2 >= 0x1f7)&&(Check2 < 0x202))
				{
				SC_Speech(14);  		// 3
				Delay(30);
				SC_Speech(21);  		// 10
				Delay(30);
				SC_Speech(17);  		// 6
				Delay(30);
				}
			else if((Check2 >= 0x202)&&(Check2 < 0x20d))
				{
				SC_Speech(15);  		// 4
				Delay(30);
				SC_Speech(21);  		// 10
				Delay(30);
				SC_Speech(15);  		// 4
				Delay(30);
				}
			else if((Check2 >= 0x20d)&&(Check2 < 0x215))
				{
				SC_Speech(16);  		// 5
				Delay(30);
				SC_Speech(21);  		// 10
				Delay(30);
				SC_Speech(13);  		// 2
				Delay(30);
				}
			else if((Check2 >= 0x215)&&(Check2 < 0x22f))
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
		else		// 60V ÁîµÁì∂
		{
			
			if(Check2 < 0x24a)
			{
				voice_EN = 1;
				SC_Speech(39);  		// ÁîµÈáè‰∏çË∂≥
				Delay(30);	
			}
			else
			{
			
				voice_EN = 1;
				SC_Speech(8);  	
				Delay(70);		
								
				if((Check2 >= 0x24a)&&(Check2 < 0x252))
					{
					SC_Speech(21);  		// 10
					Delay(30);
					}
				else if((Check2 >= 0x252)&&(Check2 < 0x259))
					{
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(15);  		// 4
					Delay(30);
					}
				else if((Check2 >= 0x259)&&(Check2 < 0x25d))
					{
					SC_Speech(13);  		// 2
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					}
				else if((Check2 >= 0x25d)&&(Check2 < 0x25f))
					{
					SC_Speech(13);  		// 2
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(17);  		// 6
					Delay(30);
					}
				else if((Check2 >= 0x25f)&&(Check2 < 0x268))
					{
					SC_Speech(14);  		// 3
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(13);  		// 2
					Delay(30);
					}
				else if((Check2 >= 0x268)&&(Check2 < 0x273))
					{
					SC_Speech(15);  		// 4
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(15);  		// 4
					Delay(30);
					}
				else if((Check2 >= 0x273)&&(Check2 < 0x27e))
					{
					SC_Speech(16);  		// 5
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(17);  		// 6
					Delay(30);
					}
				else if((Check2 >= 0x27e)&&(Check2 < 0x282))
					{
					SC_Speech(17);  		// 6
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(19);  		// 8
					Delay(30);
					}
				else if((Check2 >= 0x282)&&(Check2 < 0x287))
					{
					SC_Speech(18);  		// 7
					Delay(30);
					SC_Speech(21);  		// 10
					Delay(30);
					SC_Speech(15);  		// 4
					Delay(30);
				}
				else if(Check2 >= 0x287)
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