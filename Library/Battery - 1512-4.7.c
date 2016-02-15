/*---------------------------------------------------
	Battery.c (v1.00)
	new 201512
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
	if(Check2 < 0x240)
		Battery_level = 48;
	else if(Check2 > 0x240)
		Battery_level = 60;
		
	switch(Battery_level)
		{
		case 48:
			{
			// battery is sufficient, battery > 50.5V, 3.14V/4.7V
			if((Check2 >= 0x213))
				{
				voice_EN = 1;
				SC_Speech(20);  	
				Delay(60);
				voice_EN = 0;
				}
			// 20km, 49.5V < battery < 50.5V, 3.08V/4.93V
			else if((Check2 < 0x213)&&(Check2 >= 0x20a))		
				{
				voice_EN = 1;
				SC_Speech(8);  
				Delay(100);
				voice_EN = 0;
				}
			// 15km, 48.8V < battery < 49.5V, 3.03V/4.93V 
			else if((Check2 < 0x20a)&&(Check2 >= 0x202))		
				{
				voice_EN = 1;
				SC_Speech(7);  	
				Delay(100);
				voice_EN = 0;
				}
			// 10km, 48V < battery <48.8V, 2.99V/4.9V
			else if((Check2 < 0x202)&&(Check2 >= 0x1f9))		
				{
				voice_EN = 1;
				SC_Speech(6);  	
				Delay(100);
				voice_EN = 0;
				}
			// 5km, 46.8V < battery < 48V, 2.91V/4.9V
			else if((Check2 < 0x1f9)&&(Check2 >= 0x1ee))		
				{
				voice_EN =1;
				SC_Speech(5);  	
				Delay(100);
				voice_EN = 0;
				}                     
			// battery is low, battery < 46.8V, 
			else if(Check2 < 0x1ee)
				{
				motorBAT_low_speech();
				}			
			}
		break;
		
		case 60:
			{
			// battery is sufficient, battery > 63.5V, 3.95V/4.7V
			if((Check2 >= 0x298))
				{
				voice_EN = 1;
				SC_Speech(20);  	
				Delay(60);
				voice_EN = 0;
				}
			// 35km, 62V < battery < 63.5V, 3.86V/4.93V
			else if((Check2 < 0x298)&&(Check2 >= 0x287))		
				{
				voice_EN = 1;
				SC_Speech(9);  
				Delay(100);
				voice_EN = 0;
				}
			// 20km, 60V < battery < 62V, 3.74V/4.93V
			else if((Check2 < 0x287)&&(Check2 >= 0x273))		
				{
				voice_EN = 1;
				SC_Speech(8);  
				Delay(100);
				voice_EN = 0;
				}
			// 15km, 59.3V < battery < 60V, 3.7V/4.93V 
			else if((Check2 < 0x273)&&(Check2 >= 0x26c))		
				{
				voice_EN = 1;
				SC_Speech(7);  	
				Delay(100);
				voice_EN = 0;
				}
			// 10km, 58.3V < battery <59.3V, 3.64V/4.93V
			else if((Check2 < 0x26c)&&(Check2 >= 0x264))		
				{
				voice_EN = 1;
				SC_Speech(6);  	
				Delay(100);
				voice_EN = 0;
				}
			// 5km, 57.6V < battery < 58.3V, 3.6V/4.93V
			else if((Check2 < 0x264)&&(Check2 >= 0x25b))		
				{
				voice_EN =1;
				SC_Speech(5);  	
				Delay(100);
				voice_EN = 0;
				}                     
			// battery is low, battery < 57.6V, 
			else if(Check2 < 0x25b)
				{
				motorBAT_low_speech();
				}
			}
		}
	#endif
	
	//------------ Withbattery_48V & 60V ------------------------	
	#ifdef Z3
	if(Check2 < 0x240)
		Battery_level = 48;
	else if(Check2 > 0x240)
		Battery_level = 60;
		
	switch(Battery_level)
		{
		case 48:
			{
			// battery is sufficient, battery > 50.5V, 3.14V/4.7V
			if((Check2 >= 0x213))
				{
				voice_EN = 1;
				SC_Speech(20);  	
				Delay(60);
				voice_EN = 0;
				}
			// 20km, 49.5V < battery < 50.5V, 3.08V/4.93V
			else if((Check2 < 0x213)&&(Check2 >= 0x20a))		
				{
				voice_EN = 1;
				SC_Speech(8);  
				Delay(100);
				voice_EN = 0;
				}
			// 15km, 48.8V < battery < 49.5V, 3.03V/4.93V 
			else if((Check2 < 0x20a)&&(Check2 >= 0x202))		
				{
				voice_EN = 1;
				SC_Speech(7);  	
				Delay(100);
				voice_EN = 0;
				}
			// 10km, 48V < battery <48.8V, 2.99V/4.9V
			else if((Check2 < 0x202)&&(Check2 >= 0x1f9))		
				{
				voice_EN = 1;
				SC_Speech(6);  	
				Delay(100);
				voice_EN = 0;
				}
			// 5km, 46.8V < battery < 48V, 2.91V/4.9V
			else if((Check2 < 0x1f9)&&(Check2 >= 0x1ee))		
				{
				voice_EN =1;
				SC_Speech(5);  	
				Delay(100);
				voice_EN = 0;
				}                     
			// battery is low, battery < 46.8V, 
			else if(Check2 < 0x1ee)
				{
				motorBAT_low_speech();
				}			
			}
		break;
		
		case 60:
			{
			// battery is sufficient, battery > 63.5V, 3.95V/4.7V
			if((Check2 >= 0x298))
				{
				voice_EN = 1;
				SC_Speech(20);  	
				Delay(60);
				voice_EN = 0;
				}
			// 35km, 62V < battery < 63.5V, 3.86V/4.93V
			else if((Check2 < 0x298)&&(Check2 >= 0x287))		
				{
				voice_EN = 1;
				SC_Speech(9);  
				Delay(100);
				voice_EN = 0;
				}
			// 20km, 60V < battery < 62V, 3.74V/4.93V
			else if((Check2 < 0x287)&&(Check2 >= 0x273))		
				{
				voice_EN = 1;
				SC_Speech(8);  
				Delay(100);
				voice_EN = 0;
				}
			// 15km, 59.3V < battery < 60V, 3.7V/4.93V 
			else if((Check2 < 0x273)&&(Check2 >= 0x26c))		
				{
				voice_EN = 1;
				SC_Speech(7);  	
				Delay(100);
				voice_EN = 0;
				}
			// 10km, 58.3V < battery <59.3V, 3.64V/4.93V
			else if((Check2 < 0x26c)&&(Check2 >= 0x264))		
				{
				voice_EN = 1;
				SC_Speech(6);  	
				Delay(100);
				voice_EN = 0;
				}
			// 5km, 57.6V < battery < 58.3V, 3.6V/4.93V
			else if((Check2 < 0x264)&&(Check2 >= 0x25b))		
				{
				voice_EN =1;
				SC_Speech(5);  	
				Delay(100);
				voice_EN = 0;
				}                     
			// battery is low, battery < 57.6V, 
			else if(Check2 < 0x25b)
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