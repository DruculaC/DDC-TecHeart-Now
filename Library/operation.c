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
#include "ISP_DataFlash.h"

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
extern bit EN_host_stolen_alarming;     
extern bit host_stolen_alarm2_EN;      
extern tByte host_stolen_alarm1_count;		
extern tByte host_stolen_alarm2_count;		
extern bit Host_stolen_alarming;					
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
extern tByte Open_action_flag;		
extern tByte ID_certificated_numbers;
extern bit ID_certificated_flag;			
extern tByte After_IDcert_timecount;		
extern bit never_alarm;
extern bit IDkey_selflearn_flag1;
extern bit IDkey_selflearn_flag2;
extern bit IDkey_selflearn_flag3;
extern bit IDkey_selflearn_flag4;
extern bit IDkey_selflearn_flag5;
extern bit ID_selflearning_flag;
extern tByte ID_selflearning_timecount;
extern tWord IDkey_selflearn_HVcount;
extern tWord IDkey_selflearn_LVcount;
extern tByte fell_alarm_count;
extern tByte raised_alarm_count;
extern tByte Check_Motobattery_count;
extern bit Check_Motobattery_flag;
extern tWord load_battery_result;
extern bit ID_speeched_flag;
extern bit slave_nearby_actioned_flag;
extern bit IDkey_speech_flash;
extern bit wire_broken_reset;
extern tByte Stolen_alarm_reset_count;
extern bit vibration_flag;
extern tByte slave_nearby_count;
extern bit wheeled_flag;
extern bit IDkey_flash_EN;
extern bit flashing_flag;
extern tWord vibration_count;
extern tWord wheeled_count;
extern bit Just_power_up;

/*-----------------------------------------
	slave_away_operation()
	
	operation for slave is away
------------------------------------------*/
void slave_away_operation(void)
	{	
	if(Silence_Flag == 0)
		{
		close_lock_speech();	
		
		Check_Motobattery_flag = 1;
		Check_Motobattery_count = 0;
	   }
	// Enable_sensor();	
	enable_sensor_delayEN = 1;
	enable_sensor_delay_count = 0;
	// delay time, avoid sensor trigger on.
	Delay(20);
	ID_certificated_numbers = 0;

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
	After_IDcert_timecount = 0;
	ID_certificated_flag = 0;
	ID_certificated_numbers = 0;		

	if(Silence_Flag == 0)
		{
		open_lock_speech();
		if(Just_power_up == 0)
			verifybattery(load_battery_result);
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
	// if host has been touched 3 times, alarm 2 speeches alternantively.
	if(EN_host_stolen_alarming == 1)
		{
		Host_stolen_alarming = 1;
		
		stolen_alarm_speech1();
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
		}
	}

/*------------------------------------------------------------------
	ENsensor_afterIDcert()
	6s After ID certificated, enable sensor. 
	If ID certficated 6 times, don't enable sensor.
------------------------------------------------------------------*/
void ENsensor_afterIDcert(void)
	{
	if(ID_certificated_flag == 1)
		{
		// 3 tickets after ID certificated.
		if(++After_IDcert_timecount >= 4)
			{
			ID_certificated_flag = 0;
			After_IDcert_timecount = 0;			
			ID_certificated_numbers = 0;
			
			// 
			if((Open_action_flag == 0)&&(never_alarm == 0))
				{
				Enable_sensor();					
				}				
			never_alarm = 0;
			
			}			
		}
	}

/*------------------------------------------------------------------
	ENsensor_After_Close()
------------------------------------------------------------------*/
void ENsensor_After_Close(void)
	{
	if((enable_sensor_delayEN == 1)&&(key_rotate == 0))
		{
		if(++enable_sensor_delay_count >= 3)
			{
			enable_sensor_delay_count = 0;
			enable_sensor_delayEN = 0;
			if(never_alarm == 0)
				Enable_sensor();
			}
		}	
	}

/*-----------------------------------------------------------------
	SelfLearn_Reset()
	PIN22 triggles 3 times, the system enters selflearn mode
------------------------------------------------------------------*/
void SelfLearn_Reset(void)
	{	
	if(ID_selflearning_flag == 1)
		{
		if(++ID_selflearning_timecount > 10)
			{
			ID_selflearning_flag = 0;
			ID_selflearning_timecount = 0;				
			// Reset relatively flag and count.
			IDkey_selflearn_HVcount = 0;
			IDkey_selflearn_LVcount = 0;
			IDkey_selflearn_flag1 = 0;
			IDkey_selflearn_flag2 = 0;
			IDkey_selflearn_flag3 = 0;
			IDkey_selflearn_flag4 = 0;
			IDkey_selflearn_flag5 = 0;
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

/*----------------------------------------------------
	Disable_sensor_after_IDcert()
-----------------------------------------------------*/
void Disable_sensor_after_IDcert(void)
	{
	if(IDkey_speech_flash == 1)
		{
		IDkey_speech_flash = 0;

		disable_sensor();
		}	
	}

/*----------------------------------------------------
	Reset_after_wirebroken()
-----------------------------------------------------*/
void Reset_after_wirebroken(void)
	{
	if(wire_broken_reset == 1)
		{
		wire_broken_reset = 0;
		host_stolen_alarm1_count = 0;
		EN_host_stolen_alarming = 0;				
		Host_stolen_alarming = 0;
		sensor_3rdalarm_flag = 0;
		}
	}

/*----------------------------------------------------
	Reset_after_stolen_alarming()
	15 tickets after stolen alarm, shut alarm.
-----------------------------------------------------*/
void Reset_after_stolen_alarming(void)
	{
	if(EN_host_stolen_alarming == 1)
		{
		if(++Stolen_alarm_reset_count > 15)
			{
			host_stolen_alarm1_count = 0;
			EN_host_stolen_alarming = 0;
			Host_stolen_alarming = 0;
			sensor_3rdalarm_flag = 0;
			Stolen_alarm_reset_count = 0;
			close_tranceiver();
			}
		}
	}

/*-----------------------------------------------------
	Ensensor_after_slave_away()
------------------------------------------------------*/
void Ensensor_after_slave_away(void)
	{
	if((vibration_flag == 0)&&(wheeled_flag == 0)&&(Just_power_up == 0))
		{
		if(++slave_nearby_count > 3)
			{
			slave_nearby_count = 5;
			slave_nearby_actioned_flag = 0;
			ID_certificated_flag = 0;
			if(never_alarm == 0)
				Enable_sensor();
			}
		}	
	}

/*----------------------------------------------------
	Detect_selflearn_action()
-----------------------------------------------------*/
void Detect_selflearn_action(void)
	{
	if(key_rotate == 1)
		{
		if(wire_broken == 1)
			{
			IDkey_selflearn_LVcount = 0;
						
			if(++IDkey_selflearn_HVcount > 4000)
				{
				IDkey_selflearn_HVcount = 4002;
				IDkey_selflearn_flag1 = 0;
				IDkey_selflearn_flag2 = 0;
				IDkey_selflearn_flag3 = 0;
				IDkey_selflearn_flag4 = 0;
				IDkey_selflearn_flag5 = 0;
				}
			else
				{
				IDkey_selflearn_flag1 = 1;
				if(IDkey_selflearn_flag2 == 1)
					IDkey_selflearn_flag3 = 1;
				if(IDkey_selflearn_flag4 == 1)
					IDkey_selflearn_flag5 = 1;
				}
			}
		else
			{
			IDkey_selflearn_HVcount = 0;
			
			if(IDkey_selflearn_flag1 == 1)
				IDkey_selflearn_flag2 = 1;
				
			if(IDkey_selflearn_flag3 == 1)
				IDkey_selflearn_flag4 = 1;			
			
			if(IDkey_selflearn_flag5 == 1)
				{
				ID_selflearning_flag = 1;
				IDkey_selflearn_flag5 = 0;
				Self_learn_speech();
				}
				
			if(++IDkey_selflearn_LVcount > 4000)
				{
				IDkey_selflearn_LVcount = 4002;
				IDkey_selflearn_flag1 = 0;
				IDkey_selflearn_flag2 = 0;
				IDkey_selflearn_flag3 = 0;
				IDkey_selflearn_flag4 = 0;
				IDkey_selflearn_flag5 = 0;
				ID_selflearning_flag = 0;
				}
			}		
		}

	if(IDkey_flash_EN == 1)
		{
		IDkey_flash_EN = 0;
		flashing_flag = 1;
		IDkey_selflearn_flag1 = 0;
		IDkey_selflearn_flag2 = 0;
		IDkey_selflearn_flag3 = 0;
		IDkey_selflearn_flag4 = 0;
		IDkey_selflearn_flag5 = 0;
		ID_selflearning_flag = 0;
		
		Self_learn_programming();
		
		Self_learn_speech();
		
		#if (defined Z3) && (defined ID)
		UART_Send_Data_match();
		#endif
		}	
	}

/*---------------------------------------------------
	Detect_open_action()
---------------------------------------------------*/
void Detect_open_action(void)
	{
//	if((key_rotate == 1)&&(Open_action_flag == 0)&&(ID_certificated_flag == 1)&&(never_alarm == 0))		
	if((key_rotate == 1)&&(Open_action_flag == 0)&&(ID_certificated_flag == 1))		
		{
		disable_sensor();
		Open_action_flag = 1;
		ID_speeched_flag = 0;
		
		After_IDcert_timecount = 0;
		ID_certificated_flag = 0;
		ID_certificated_numbers = 0;
		slave_nearby_actioned_flag = 1;
		ElecMotor_CW();
		slave_nearby_operation();
		
		Just_power_up = 0;
		} 			
	}

/*---------------------------------------------------
	Detect_close_action()
---------------------------------------------------*/
void Detect_close_action(void)
	{
	if(((key_rotate == 0)||(slave_nearby_actioned_flag == 0))&&(Open_action_flag == 1))
		{
		if((vibration_flag == 0)&&(wheeled_flag == 0))
			{
			Delay_1ms();
			if((key_rotate == 0)||(slave_nearby_actioned_flag == 0))
				{
				ElecMotor_ACW();

				Open_action_flag = 0;
				slave_away_operation();		
				IDkey_speech_flash = 0;
				ID_speeched_flag = 0;
				}				
			}
		}	
	}

/*------------------------------------------------------
	Detect_vibration()
-------------------------------------------------------*/
void Detect_vibration(void)
	{
	if((sensor_detect == 0)||(horizontal_sensor == 0))
		{
		vibration_flag = 1;
		vibration_count = 0;
		}
    if(vibration_flag == 1)
		{
		if(++vibration_count >= 2000)
			{
			vibration_flag = 0;
			vibration_count = 0;
			}
		}	
	}

/*----------------------------------------------------
	Detect_wheel_moving()
-----------------------------------------------------*/
void Detect_wheel_moving(void)
	{
	if(wheeled_rotate == 1)
		{
		wheeled_flag = 1;
		wheeled_count = 0;
		}
	if(wheeled_flag == 1)
		{
		if(++wheeled_count >= 2000)
			{
			wheeled_flag = 0;
			wheeled_count = 0;
         }
		}	
	}

/*---------------------------------------------------
	end of file
----------------------------------------------------*/
