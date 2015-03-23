/*---------------------------------------------------
	Battery.c (v1.00)
	
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

/*----------------------------------------------------
	CheckADC() - 48V 电平
	Check P20's voltage.
-----------------------------------------------------*/
void CheckADC(void)
	{
	// detect the battery voltage
	ADC_check_result = GetADCResult(6);
	}

                                                         
/*----------------------------------------------------
	verifybattery() - Z2 主板不带电池
	voice alarm for battery status without power module
void verifybattery(tWord Check2)
	{	
	// battery is sufficient, battery > 50.5V, 3.72V/4.93V
	if((Check2 >= 0x304))
		{
		voice_EN = 1;
		SC_Speech(18);  	
		Delay(60);
		voice_EN = 0;
		}
   // 20km, 49.5V < battery < 50.5V, 3.68V/4.93V
	else if((Check2 < 0x304)&&(Check2 >= 0x2fc))		
		{
		voice_EN = 1;
		SC_Speech(8);  
		Delay(100);
		voice_EN = 0;
		}
	// 15km, 48.8V < battery < 49.5V, 3.65V/4.93V 
	else if((Check2 < 0x2fc)&&(Check2 >= 0x2f6))		
		{
		voice_EN = 1;
		SC_Speech(7);  	
		Delay(100);
		voice_EN = 0;
		}
	// 10km, 48V < battery <48.8V, 3.61V/4.9V
	else if((Check2 < 0x2f6)&&(Check2 >= 0x2ed))		
		{
		voice_EN = 1;
		SC_Speech(6);  	
		Delay(100);
		voice_EN = 0;
		}
	// 5km, 46.8V < battery < 48V, 3.55V/4.9V
	else if((Check2 < 0x2ed)&&(Check2 >= 0x2e1))		
		{
		voice_EN =1;
		SC_Speech(5);  	
		Delay(100);
		voice_EN = 0;
		}                     
	// battery is low, battery < 46.8V, 
	else if(Check2 < 0x2e1)
		{
		motorBAT_low_speech();
		}
	}
-----------------------------------------------------*/

/*----------------------------------------------------
	verifybattery() - Z2   主板带电池
	voice alarm for battery status without power module
void verifybattery(tWord Check2)
	{	
	// battery is sufficient, battery > 51.5V, 3.19V/4.1V
	if(Check2 >= 0x31c)
		{
		voice_EN = 1;
		SC_Speech(18);  	
		Delay(60);
		voice_EN = 0;
		Check2_stage = 5;
		}
	// 15km, 50.6V < battery < 51.5V, 3.13V/4.1V 
	else if((Check2 < 0x31c)&&(Check2 >= 0x30d))		
		{
		voice_EN = 1;
		SC_Speech(7);  	
		Delay(100);
		voice_EN = 0;
		Check2_stage = 3;
		}
	// 5km, 48.5V < battery < 50.6V, 3.00V/4.1V 
	else if((Check2 < 0x30d)&&(Check2 >= 0x2ed))		
		{
		voice_EN = 1;
		SC_Speech(5);  	
		Delay(100);
		voice_EN = 0;
		Check2_stage = 3;
		}
	// battery is low, battery < 48.5V,    3.00V/4.1V
	else if(Check2 < 0x2ed)
		{
		motorBAT_low_speech();
		Check2_stage = 0;
		}
	}
 -----------------------------------------------------*/

/*----------------------------------------------------
	verifybattery_anytime() - Z2   主板带电池
	voice alarm for battery status without power module
void verifybattery_anytime(tWord Check2)
	{	
	// battery is sufficient, battery > 50.5V, 3.13V/4.1V
	if((Check2 >= 0x30d)&&(Check2_stage != 5))
		{
		voice_EN = 1;
		SC_Speech(18);  	
		Delay(60);
		voice_EN = 0;
		Check2_stage = 5;
		}
   // 20km, 49.5V < battery < 50.5V, 3.07V/4.1V
	else if((Check2 < 0x30d)&&(Check2 >= 0x2fe)&&(Check2_stage != 4))		
		{
		voice_EN = 1;
		SC_Speech(8);  
		Delay(100);
		voice_EN = 0;
		Check2_stage = 4;
		}
	// 10km, 48V < battery <48.8V, 2.98V/4.1V
	else if((Check2 < 0x2fe)&&(Check2 >= 0x2e8)&&(Check2_stage != 2))		
		{
		voice_EN = 1;
		SC_Speech(6);  	
		Delay(100);
		voice_EN = 0;
		Check2_stage = 2;
		}
	// battery is low, battery < 46.8V, 
	else if((Check2 < 0x2e8)&&(Check2_stage != 0))
		{
		motorBAT_low_speech();
		Check2_stage = 0;
		}
	}
-----------------------------------------------------*/

/*----------------------------------------------------
	verifybattery()  - 60V 电平
	voice alarm for battery status with power module
-----------------------------------------------------*/

void verifybattery(tWord Check2)
	{	
	// battery is sufficient, battery > 63.5V, 3.95V/4.1V
	if((Check2 >= 0x3da))
		{
		voice_EN = 1;
		SC_Speech(18);  	
		Delay(60);
		voice_EN = 0;
		}
   // 35km, 62V < battery < 63.5V, 3.86V/4.1V
	else if((Check2 < 0x3da)&&(Check2 >= 0x3c4))		
		{
		voice_EN = 1;
		SC_Speech(9);  
		Delay(100);
		voice_EN = 0;
		}
   // 20km, 60V < battery < 62V, 3.74V/4.1V
	else if((Check2 < 0x3c4)&&(Check2 >= 0x3a6))		
		{
		voice_EN = 1;
		SC_Speech(8);  
		Delay(100);
		voice_EN = 0;
		}
	// 15km, 59.3V < battery < 60V, 3.7V/4.1V 
	else if((Check2 < 0x3a6)&&(Check2 >= 0x39c))		
		{
		voice_EN = 1;
		SC_Speech(7);  	
		Delay(100);
		voice_EN = 0;
		}
	// 10km, 58.3V < battery <59.3V, 3.64V/4.1V
	else if((Check2 < 0x39c)&&(Check2 >= 0x38d))		
		{
		voice_EN = 1;
		SC_Speech(6);  	
		Delay(100);
		voice_EN = 0;
		}
	// 5km, 57.6V < battery < 58.3V, 3.6V/4.1V
	else if((Check2 < 0x38d)&&(Check2 >= 0x383))		
		{
		voice_EN =1;
		SC_Speech(5);  	
		Delay(100);
		voice_EN = 0;
		}                     
	// battery is low, battery < 57.6V, 
	else if(Check2 < 0x383)
		{
		motorBAT_low_speech();
		}
	}
/*---------------------------------------------------
	end of file
----------------------------------------------------*/