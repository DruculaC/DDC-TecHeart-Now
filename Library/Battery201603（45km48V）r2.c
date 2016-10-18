/*---------------------------------------------------
	Battery.c (v1.00)
	Battery201603（45km48V）r1
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
	Check_motor_accumulator() - 48V 电平
	Check P20's voltage.
-----------------------------------------------------*/
void Check_motor_accumulator(void)
	{
	// detect the battery voltage
	ADC_check_result = GetADCResult(6);
	}


/*-----------------------------------------
	Broadcast_battery()
	
	载入电量值时，如果新的电量值小于原先的值，或者新电量值大于最大电量值，则载入进去播报。否则视为浮点
	不播报。
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
	// 电量还能行驶
	voice_EN = 1;
	SC_Speech(8);  	
	Delay(70);	
	// 多少
	if(Check2 < 0x1e5)
		{
		SC_Speech(13);  		// 2
		Delay(30);
		}
	else if((Check2 >= 0x1e5)&&(Check2 < 0x1f6))
		{
		SC_Speech(16);  		// 5
		Delay(30);
		}
	else if((Check2 >= 0x1f6)&&(Check2 < 0x1fa))
		{
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(12);  		// 1
		Delay(30);
		}
	else if((Check2 >= 0x1fa)&&(Check2 < 0x202))
		{
		SC_Speech(13);  		// 2
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(12);  		// 1
		Delay(30);
		}
	else if((Check2 >= 0x202)&&(Check2 < 0x206))
		{
		SC_Speech(13);  		// 2
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(17);  		// 6
		Delay(30);
		}
	else if((Check2 >= 0x206)&&(Check2 < 0x20c))
		{
		SC_Speech(14);  		// 3
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(12);  		// 1
		Delay(30);
		}
	else if((Check2 >= 0x20c)&&(Check2 < 0x211))
		{
		SC_Speech(15);  		// 4
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		}
	else if(Check2 >= Max_battery)
		{
		SC_Speech(15);  		// 4
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(16);  		// 5
		Delay(30);
		}		
	// 公里
	SC_Speech(11);  	
	Delay(60);	
	voice_EN = 0;
	SC_RST = 1;
	Delay_50ms();
	SC_RST = 0;
	Delay_50ms();
	#endif
	}
	
/*---------------------------------------------------
	end of file
----------------------------------------------------*/