/*---------------------------------------------------
	Battery.c (v1.00)
<<<<<<< HEAD
	Battery201603（70km60V）r1
=======
	Battery201603（90km72V）r1
>>>>>>> c6e65e4f0b4864e969ea86603842dc8b6ee1929b
---------------------------------------------------*/

#include "main.h"
#include "port.h"

#include "Battery.h"
#include "voice.h"
#include "Delay.h"
#include "AD.h"
<<<<<<< HEAD
#define Max_battery 0x291
=======
#define Max_battery 0x317
>>>>>>> c6e65e4f0b4864e969ea86603842dc8b6ee1929b

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
	
	你好
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
<<<<<<< HEAD
	if(Check2<0x25f)
		{
		SC_Speech(39);  		// 鐢甸噺涓嶈冻
		Delay(50);
		}
	else if(Check2 > 0x291)
		{		
		SC_Speech(8);  	
		Delay(70);			
		SC_Speech(18);  		// 7
		Delay(30);
=======
	SC_Speech(8);  	
	Delay(70);	
	// 多少
	if(Check2 < 0x2a8)
		{
		SC_Speech(14);  		// 3
		Delay(30);
		}
	else if((Check2 >= 0x2a8)&&(Check2 < 0x2b6))
		{
		SC_Speech(18);			// 7
		Delay(30);
		}
	else if((Check2 >= 0x2b6)&&(Check2 < 0x2c7))
		{
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(16);  		// 5
		Delay(30);
		}
	else if((Check2 >= 0x2c7)&&(Check2 < 0x2d8))
		{
		SC_Speech(13);  		// 2
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(12);  		// 1
		Delay(30);
		}
	else if((Check2 >= 0x2d8)&&(Check2 < 0x2e2))
		{
		SC_Speech(13);  		// 2
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(19);  		// 8
		Delay(30);
		}
	else if((Check2 >= 0x2e2)&&(Check2 < 0x2ec))
		{
		SC_Speech(14);  		// 3
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(17);  		// 6
		Delay(30);
		}
	else if((Check2 >= 0x2ec)&&(Check2 < 0x2f7))
		{
		SC_Speech(15);  		// 4
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(14);  		// 3
		Delay(30);
		}
	else if((Check2 >= 0x2f7)&&(Check2 < 0x302))
		{
		SC_Speech(16);  		// 5
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(12);  		// 1
		Delay(30);
		}
	else if((Check2 >= 0x302)&&(Check2 < 0x30c))
		{
		SC_Speech(17);  		// 6
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		}
	else if((Check2 >= 0x30c)&&(Check2 < 0x311))
		{
		SC_Speech(17);  		// 6
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(18);  		// 7
		Delay(30);
		}
	else if((Check2 >= 0x311)&&(Check2 < 0x315))
		{
		SC_Speech(18);  		// 7
		Delay(30);
>>>>>>> c6e65e4f0b4864e969ea86603842dc8b6ee1929b
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(11);  	
		Delay(60);	
		}
<<<<<<< HEAD
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
=======
	else if((Check2 >= 0x315)&&(Check2 < 0x317))
		{
		SC_Speech(19);  		// 8
		Delay(30);
		SC_Speech(21);  		// 10
		Delay(30);
		SC_Speech(13);  		// 2
		Delay(30);
		}
	else if(Check2 >= 0x317)
		{
		SC_Speech(20);  		// 9
		Delay(30);
		SC_Speech(21);  		// 10
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
>>>>>>> c6e65e4f0b4864e969ea86603842dc8b6ee1929b
	#endif
	}
	
/*---------------------------------------------------
	end of file
----------------------------------------------------*/