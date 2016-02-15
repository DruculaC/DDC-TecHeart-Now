/*---------------------------------------------------
	Battery.c (v1.00)
	old
	电池电量判断
---------------------------------------------------*/	

#include "main.h"
#include "port.h"

#include "Battery.h"
#include "voice.h"
#include "Delay.h"
#include "AD.h"

extern tWord ADC_check_result;		
tByte Check2_stage = 0;
tByte Battery_level = 0;

/*----------------------------------------------------
	Check_motor_accumulator() - 48V 电平
	Check P20's voltage.
-----------------------------------------------------*/
void Check_motor_accumulator(void)
	{
	// detect the battery voltage
	ADC_check_result = GetADCResult(6);
	}


/*----------------------------------------------------
	verifybattery()
	voice alarm for battery status with power module
-----------------------------------------------------*/
void verifybattery(tWord Check2)
	{	
	//------------ Withoutbattery_48V & 60V ------------------------	
	#ifdef Z2
	if(Check2 < 0x21f)
		Battery_level = 48;
	else if(Check2 > 0x224)
		Battery_level = 60;
		
	switch(Battery_level)
		{
		case 48:
			{
			// battery is sufficient, battery > 50.5V, 3.14V/4.93V
			if((Check2 >= 0x1ff))
				{
				voice_EN = 1;
				SC_Speech(18);  	
				Delay(60);
				voice_EN = 0;
				}
			// 20km, 49.5V < battery < 50.5V, 3.08V/4.93V
			else if((Check2 < 0x1ff)&&(Check2 >= 0x1f7))		
				{
				voice_EN = 1;
				SC_Speech(8);  
				Delay(100);
				voice_EN = 0;
				}
			// 15km, 48.8V < battery < 49.5V, 3.03V/4.93V 
			else if((Check2 < 0x1f7)&&(Check2 >= 0x1ef))		
				{
				voice_EN = 1;
				SC_Speech(7);  	
				Delay(100);
				voice_EN = 0;
				}
			// 10km, 48V < battery <48.8V, 2.99V/4.9V
			else if((Check2 < 0x1ef)&&(Check2 >= 0x1e6))		
				{
				voice_EN = 1;
				SC_Speech(6);  	
				Delay(100);
				voice_EN = 0;
				}
			// 5km, 46.8V < battery < 48V, 2.91V/4.9V
			else if((Check2 < 0x1e6)&&(Check2 >= 0x1df))		
				{
				voice_EN =1;
				SC_Speech(5);  	
				Delay(100);
				voice_EN = 0;
				}                     
			// battery is low, battery < 46.8V, 
			else if(Check2 < 0x1df)
				{
				motorBAT_low_speech();
				}			
			}
		break;
		
		case 60:
			{
			// battery is sufficient, battery > 63.5V, 3.95V/4.93V
			if((Check2 >= 0x27e))
				{
				voice_EN = 1;
				SC_Speech(18);  	
				Delay(60);
				voice_EN = 0;
				}
			// 35km, 62V < battery < 63.5V, 3.86V/4.93V
			else if((Check2 < 0x27e)&&(Check2 >= 0x26e))		
				{
				voice_EN = 1;
				SC_Speech(9);  
				Delay(100);
				voice_EN = 0;
				}
			// 20km, 60V < battery < 62V, 3.74V/4.93V
			else if((Check2 < 0x26e)&&(Check2 >= 0x25b))		
				{
				voice_EN = 1;
				SC_Speech(8);  
				Delay(100);
				voice_EN = 0;
				}
			// 15km, 59.3V < battery < 60V, 3.7V/4.93V 
			else if((Check2 < 0x25b)&&(Check2 >= 0x255))		
				{
				voice_EN = 1;
				SC_Speech(7);  	
				Delay(100);
				voice_EN = 0;
				}
			// 10km, 58.3V < battery <59.3V, 3.64V/4.93V
			else if((Check2 < 0x255)&&(Check2 >= 0x24c))		
				{
				voice_EN = 1;
				SC_Speech(6);  	
				Delay(100);
				voice_EN = 0;
				}
			// 5km, 57.6V < battery < 58.3V, 3.6V/4.93V
			else if((Check2 < 0x24c)&&(Check2 >= 0x244))		
				{
				voice_EN =1;
				SC_Speech(5);  	
				Delay(100);
				voice_EN = 0;
				}                     
			// battery is low, battery < 57.6V, 
			else if(Check2 < 0x244)
				{
				motorBAT_low_speech();
				}
			}
		}
	#endif
	
	//------------ Withbattery_48V & 60V ------------------------	
	#ifdef Z3
	if(Check2 < 0x2a0)
		Battery_level = 48;
	else if(Check2 > 0x2a1)
		Battery_level = 60;
		
	switch(Battery_level)
		{
		case 48:
			{
			// battery is sufficient, battery > 50.5V, 3.14V/4.93V
			if((Check2 >= 0x261))
				{
				voice_EN = 1;
				SC_Speech(18);  	
				Delay(60);
				voice_EN = 0;
				}
			// 20km, 49.5V < battery < 50.5V, 3.08V/4.93V
			else if((Check2 < 0x261)&&(Check2 >= 0x257))		
				{
				voice_EN = 1;
				SC_Speech(8);  
				Delay(100);
				voice_EN = 0;
				}
			// 15km, 48.8V < battery < 49.5V, 3.03V/4.93V 
			else if((Check2 < 0x257)&&(Check2 >= 0x24d))		
				{
				voice_EN = 1;
				SC_Speech(7);  	
				Delay(100);
				voice_EN = 0;
				}
			// 10km, 48V < battery <48.8V, 2.99V/4.9V
			else if((Check2 < 0x24d)&&(Check2 >= 0x243))		
				{
				voice_EN = 1;
				SC_Speech(6);  	
				Delay(100);
				voice_EN = 0;
				}
			// 5km, 46.8V < battery < 48V, 2.91V/4.9V
			else if((Check2 < 0x243)&&(Check2 >= 0x236))		
				{
				voice_EN =1;
				SC_Speech(5);  	
				Delay(100);
				voice_EN = 0;
				}                     
			// battery is low, battery < 46.8V, 
			else if(Check2 < 0x236)
				{
				motorBAT_low_speech();
				}			
			}
		break;
		
		case 60:
			{
			// battery is sufficient, battery > 63.5V, 3.95V/4.93V
			if((Check2 >= 0x2f9))
				{
				voice_EN = 1;
				SC_Speech(18);  	
				Delay(60);
				voice_EN = 0;
				}
			// 35km, 62V < battery < 63.5V, 3.86V/4.93V
			else if((Check2 < 0x2f9)&&(Check2 >= 0x2e5))		
				{
				voice_EN = 1;
				SC_Speech(9);  
				Delay(100);
				voice_EN = 0;
				}
			// 20km, 60V < battery < 62V, 3.74V/4.93V
			else if((Check2 < 0x2e5)&&(Check2 >= 0x2cf))		
				{
				voice_EN = 1;
				SC_Speech(8);  
				Delay(100);
				voice_EN = 0;
				}
			// 15km, 59.3V < battery < 60V, 3.7V/4.93V 
			else if((Check2 < 0x2cf)&&(Check2 >= 0x2c7))		
				{
				voice_EN = 1;
				SC_Speech(7);  	
				Delay(100);
				voice_EN = 0;
				}
			// 10km, 58.3V < battery <59.3V, 3.64V/4.93V
			else if((Check2 < 0x2c7)&&(Check2 >= 0x2bd))		
				{
				voice_EN = 1;
				SC_Speech(6);  	
				Delay(100);
				voice_EN = 0;
				}
			// 5km, 57.6V < battery < 58.3V, 3.6V/4.93V
			else if((Check2 < 0x2bd)&&(Check2 >= 0x2b3))		
				{
				voice_EN =1;
				SC_Speech(5);  	
				Delay(100);
				voice_EN = 0;
				}                     
			// battery is low, battery < 57.6V, 
			else if(Check2 < 0x2b3)
				{
				motorBAT_low_speech();
				}
			}
		}
	#endif
	}
/*---------------------------------------------------
	end of file
----------------------------------------------------*/