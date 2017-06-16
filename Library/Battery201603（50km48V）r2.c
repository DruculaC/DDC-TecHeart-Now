/*---------------------------------------------------
	Battery.c (v1.00)
	Battery201603��70km60V��r1
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
	Check_motor_accumulator() - 48V ��ƽ
	Check P20's voltage.
-----------------------------------------------------*/
void Check_motor_accumulator(void)
	{
	// detect the battery voltage
	ADC_check_result = GetADCResult(6);
	}
/*-----------------------------------------
	Broadcast_battery()
	
	���
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
	#ifdef Kilometer_65
	// 
	voice_EN = 1;
	if(Check2<0x1e5)
		{
		SC_Speech(39);  		// 电量不足
		Delay(50);
		}
	else if(Check2 > 0x20c)
		{		
		SC_Speech(8);  	
		Delay(70);			
		SC_Speech(16);  		// 5
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(11);  	
		Delay(60);	
		}
		voice_EN = 0;
		SC_RST = 1;
		Delay_50ms();
		SC_RST = 0;
		Delay_50ms();

/*		if(Check2 < 0x242)
			{
			SC_Speech(13);  		// 2
			Delay(30);
			}
		else if((Check2 >= 0x242)&&(Check2 < 0x250))
			{
			SC_Speech(16);			// 5
			Delay(30);
			}
		else if((Check2 >= 0x250)&&(Check2 < 0x25f))
			{
			SC_Speech(21);  		// 10
			Delay(30);
			}
	else if((Check2 >= 0x250)&&(Check2 < 0x25f))
			{
			SC_Speech(21);  		// 10
			Delay(30);
			SC_Speech(17);  		// 6
			Delay(30);
			}
		else if((Check2 >= 0x25f)&&(Check2 < 0x267))
			{
			SC_Speech(13);  		// 2
			Delay(30);
			SC_Speech(21);  		// 10
			Delay(30);
			SC_Speech(13);  		// 2
			Delay(30);
			}
		else if((Check2 >= 0x267)&&(Check2 < 0x26f))
			{
			SC_Speech(13);  		// 2
			Delay(30);
			SC_Speech(21);  		// 10
			Delay(30);
			SC_Speech(19);  		// 8
			Delay(30);
			}
		else if((Check2 >= 0x26f)&&(Check2 < 0x279))
			{
			SC_Speech(14);  		// 3
			Delay(30);
			SC_Speech(21);  		// 10
			Delay(30);
			SC_Speech(15);  		// 4
			Delay(30);
			}
		else if((Check2 >= 0x279)&&(Check2 < 0x282))
			{
			SC_Speech(15);  		// 4
			Delay(30);
			SC_Speech(21);  		// 10
			Delay(30);
			}
		else if((Check2 >= 0x282)&&(Check2 < 0x28a))
			{
			SC_Speech(15);  		// 4
			Delay(30);
			SC_Speech(21);  		// 10
			Delay(30);
			SC_Speech(17);  		// 6
			Delay(30);
			}
		else if((Check2 >= 0x28a)&&(Check2 < 0x28e))
			{
			SC_Speech(16);  		// 5
			Delay(30);
			SC_Speech(21);  		// 10
			Delay(30);
			SC_Speech(13);  		// 2
			Delay(30);
			}
		else if((Check2 >= 0x28e)&&(Check2 < 0x291))
			{
			SC_Speech(16);  		// 5
			Delay(30);
			SC_Speech(21);  		// 10
			Delay(30);
			SC_Speech(19);  		// 8
			Delay(30);
			}
		else if((Check2 >= 0x291)&&(Check2 < Max_battery))
			{
			SC_Speech(17);  		// 6
			Delay(30);
			SC_Speech(21);  		// 10
			Delay(30);
			SC_Speech(15);  		// 4
			Delay(30);
			}
		else if(Check2 >= Max_battery)
			{
			SC_Speech(18);  		// 7
			Delay(30);
			SC_Speech(21);  		// 10
			Delay(30);
			}	
		//
		SC_Speech(11);  	
		Delay(60);	
	*/		
	#endif
	}
	
/*---------------------------------------------------
	end of file
----------------------------------------------------*/