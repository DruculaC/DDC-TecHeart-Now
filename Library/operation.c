/*-------------------------------------------------------------
	operation.c
	各类操作程序
--------------------------------------------------------------*/

#include "main.h"
#include "port.h"

#include "operation.h"
#include "other.h"
#include "voice.h"
#include "battery.h"
#include "delay.h"
#include "ElecMotor.h"
#include "communication.h"
#include "schedular.h"

/*------ private variable --------------------------*/
bit enable_sensor_delayEN = 0;		// 延迟使能传感器的使能，不能即时使能传感器，需要过一段时间		

bit sensor_EN = 0;

/*------- Public variable declarations --------------------------*/
extern bit position_sensor_EN;  	
extern bit fell_flag;						
extern bit raised_flag;				
extern tByte sensor_trigger_count;	
extern tByte sensor_1ststage_count;	
extern bit raised_fell_once_flag;			
extern bit raised_fell_flag;					
extern bit host_stolen_alarm1_EN;     
extern bit host_stolen_alarm2_EN;      
extern tByte host_stolen_alarm1_count;		
extern tByte host_stolen_alarm2_count;		
extern bit stolen_alarm_flag;					
extern tWord sensor_3rdstage_time;			
extern tByte sensor_3rdstage_effcount;		
extern tByte sensor_3rdstage_count;			
extern tWord sensor_3rdstage_interval;		
extern tWord sensor_2ndstage_time;		
extern tByte sensor_2ndstage_count;		
extern tWord ADC_check_result;		
extern tByte wire_broken_count;		
extern bit wire_broken_flag;			
extern bit battery_stolen_EN;			
extern tByte battery_stolen_count;
extern bit sensor_3rdalarm_flag;
extern tByte enable_sensor_delay_count;		
extern bit Silence_Flag;
extern tByte key_rotated_on_flag;		
extern tByte IDkey_certificated_times;
extern bit IDkey_flag;			
extern tByte IDkey_count;		
extern bit never_alarm;
extern bit IDkey_selflearn_flag1;
extern bit IDkey_selflearn_flag2;
extern bit IDkey_selflearn_flag3;
extern bit IDkey_selflearn_flag4;
extern bit IDkey_selflearn_flag5;
extern bit IDkey_selflearn_flag6;
extern tByte IDkey_selflearn_flag6count;
extern tWord IDkey_selflearn_HVcount;
extern tWord IDkey_selflearn_LVcount;
extern tByte fell_alarm_count;
extern tByte raised_alarm_count;
extern tByte Check_Motobattery_count;
extern bit Check_Motobattery_flag;
extern tWord load_battery_result;
extern bit ID_speeched_flag;
extern bit slave_nearby_actioned_flag;


/*-----------------------------------------
	slave_away_operation()
	
	operation for slave is away
------------------------------------------*/
void slave_away_operation(void)
	{
	
	if(Silence_Flag == 0)
		{
		close_lock_speech();	
		
		#ifdef Batterycheck
		Check_Motobattery_flag = 1;
		Check_Motobattery_count = 0;
		#endif

	   }
	// enable_sensor();	
	enable_sensor_delayEN = 1;
	enable_sensor_delay_count = 0;
	// delay time, avoid sensor trigger on.
	Delay(20);
	IDkey_certificated_times = 0;

	if(Silence_Flag == 1)
		Silence_Flag = 0;
	}

/*----------------------------------------------------------------------
		slave_nearby_operation()
		operation for slave is nearby
----------------------------------------------------------------------*/
void slave_nearby_operation(void)
	{
	slave_nearby_actioned_flag = 1;
	
	ID_speeched_flag = 0;		
	IDkey_count = 0;
	IDkey_flag = 0;
	IDkey_certificated_times = 0;		

	if(Silence_Flag == 0)
		{
		open_lock_speech();
		#ifdef Batterycheck
		verifybattery(load_battery_result);
		#endif
		key_rotate_on_speech();
		}
	}

/*------------------------------------------------------------------
	InitSensor()
	Initialise Sensor.
------------------------------------------------------------------*/
void InitSensor(void)
	{
	sensor_EN = 0;
 	position_sensor_EN = 0;
	enable_sensor_delayEN = 0;	
	raised_sensor_detect = 1;
	fell_sensor_detect = 1;	
	}
	
/*------------------------------------------------------------------
	Host_stolen_action()
	Determine host has been triggled 3 times, alarm action.
------------------------------------------------------------------*/
void Host_stolen_action(void)
	{
	// whether host has been touched 3 times, if yes, then alarm 2 speech alternantively.
	if((host_stolen_alarm1_EN == 1)&&(host_stolen_alarm1_count < 4))
		{
		stolen_alarm_flag = 1;
		
		#ifdef Z3
		if(wire_broken_flag == 0)
			{
			UART_Send_Data(ComMode_3);																			
			}
		else
			{                                                                     
			UART_Send_Data(ComMode_6);																			
			}
		#endif
		
		stolen_alarm_speech1();
		if(++host_stolen_alarm1_count >= 4)
			{
			host_stolen_alarm1_count = 0;
			host_stolen_alarm1_EN = 0;
			stolen_alarm_flag = 0;
			sensor_3rdalarm_flag = 0;
			}
		}
	if((host_stolen_alarm2_EN == 1)&&(host_stolen_alarm2_count < 4))
		{
		stolen_alarm_flag = 1;
		
		#ifdef Z3
		if(wire_broken_flag == 0)
			{
			UART_Send_Data(ComMode_3);																			
			}
		else
			{
			UART_Send_Data(ComMode_6);																			
			}
		#endif
		
		stolen_alarm_speech2();
		if(++host_stolen_alarm2_count >= 4)
			{
			host_stolen_alarm2_count = 0;
			host_stolen_alarm2_EN = 0;
			stolen_alarm_flag = 0;
			sensor_3rdalarm_flag = 0;
			}
		}	
	}

/*------------------------------------------------------------------
	ENsensor_afterIDcert()
	4s After ID certificated, enable sensor. 
	If ID certficated 6 times, don't enable sensor.
------------------------------------------------------------------*/
void ENsensor_afterIDcert(void)
	{
	if(IDkey_flag == 1)
		{
		if(++IDkey_count >= 3)
			{
			IDkey_count = 0;
			IDkey_flag = 0;
			IDkey_certificated_times = 0;
			if((key_rotated_on_flag == 0)&&(never_alarm == 0))
				{
				enable_sensor();					
				}				
			never_alarm = 0;
			}			
		}
	}

/*------------------------------------------------------------------
	ENsensor_After_CloseLock()
------------------------------------------------------------------*/
void ENsensor_After_CloseLock(void)
	{
	if((enable_sensor_delayEN == 1)&&(key_rotate == 0))
		{
		if(++enable_sensor_delay_count >= 3)
			{
			enable_sensor_delay_count = 0;
			enable_sensor_delayEN = 0;
			enable_sensor();
			}
		}	
	}

/*-----------------------------------------------------------------
	SelfLearn_Reset()
	PIN26 triggles 3 times, the system enters selflearn mode
------------------------------------------------------------------*/
void SelfLearn_Reset(void)
	{
	if(IDkey_selflearn_flag6 == 1)
		{
		if(++IDkey_selflearn_flag6count > 5)
			{
			IDkey_selflearn_HVcount = 0;
			IDkey_selflearn_LVcount = 0;
			IDkey_selflearn_flag1 = 0;
			IDkey_selflearn_flag2 = 0;
			IDkey_selflearn_flag3 = 0;
			IDkey_selflearn_flag4 = 0;
			IDkey_selflearn_flag5 = 0;
			IDkey_selflearn_flag6 = 0;
			IDkey_selflearn_flag6count = 0;				
			}			
		}
	}

/*---------------------------------------------------
	Fell_Alarm_to_Slave()
	Send fell alarm signal to remote slave.
----------------------------------------------------*/
void Fell_Alarm_to_Slave(void)
	{
	if((fell_flag==1)&&(fell_alarm_count<5))
		{
		UART_Send_Data(ComMode_5);																			
		fell_alarm_count++;
		}	
	}

/*----------------------------------------------------
	Raise_Alarm_to_Slave()
	Send raised alarm signal to remote slave
-----------------------------------------------------*/
void Raise_Alarm_to_Slave(void)
	{
	if((raised_flag==1)&&(raised_alarm_count<5))		
		{
		UART_Send_Data(ComMode_4);																			
		raised_alarm_count++;
		}			
	}

/*----------------------------------------------------
	Batstolen_Alarm_to_Slave()
	Send battery stolen alarm signal to remote slave
-----------------------------------------------------*/
void Batstolen_Alarm_to_Slave(void)
	{
	if((battery_stolen_EN == 1)&&(battery_stolen_count < 5))
		{
		UART_Send_Data(ComMode_2);																			
		battery_stolen_count++;
		}	
	}

/*-----------------------------------------------------
	Never_Alarm_Action()
	ID cert
/*---------------------------------------------------
	end of file
----------------------------------------------------*/
