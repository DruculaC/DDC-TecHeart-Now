/*---------------------------------------------------
	Other.c (v1.00)
	
	��������
---------------------------------------------------*/

#include "main.h"
#include "port.h"

#include "Other.h"
#include "Delay.h"
#include "communication.h"

/*------- Public variable declarations --------------------------*/
extern bit position_sensor_EN;  		
extern bit fell_flag;						
extern bit raised_flag;					
extern tByte sensor_trigger_count;	
extern tByte sensor_1ststage_count;	
extern tWord sensor_3rdstage_time;			
extern tByte sensor_3rdstage_effcount;		
extern tByte sensor_3rdstage_count;			
extern tWord sensor_3rdstage_interval;		
extern tWord sensor_2ndstage_time;		
extern tByte sensor_2ndstage_count;		
extern bit stolen_alarm_flag;					
extern bit host_stolen_alarm1_EN;      
extern bit host_stolen_alarm2_EN;     
extern tByte host_stolen_alarm1_count;		
extern tByte host_stolen_alarm2_count;	
extern bit raised_fell_flag;					
extern bit wire_broken_flag;			
extern tByte wire_broken_count;		
extern bit battery_stolen_EN;			
extern tByte battery_stolen_count;	
extern bit sensor_EN;
extern tByte enable_sensor_delay_count;		
extern bit enable_sensor_delayEN;

/*-------------------------------------------------------
	magnet_CW()
	�������ת��˳ʱ�룬�����򿪣�
--------------------------------------------------------*/
void magnet_CW(tWord x, y, tByte z)
	{
	// ���Ϳ�������
	send_code_to_lock(ComMode_1, z);
	
	MagentControl_1 = 1;
	MagentControl_2 = 0;
	Magnet_Delay(x, y);
	MagentControl_1 = 1;
	MagentControl_2 = 1;
	Externalmotor = 0;
	}

/*-------------------------------------------------------
	magnet_CW_Taili()
	�������ת��˳ʱ�룬�����򿪣�
--------------------------------------------------------*/
void magnet_CW_Taili(tWord x, y, tByte z)
	{
	// ���Ϳ�������
	send_code_to_lock_Taili(ComMode_1, z);
	
	MagentControl_1 = 1;
	MagentControl_2 = 0;
	Magnet_Delay(x, y);
	MagentControl_1 = 0;
	MagentControl_2 = 0;
	Externalmotor = 0;
	}

/*-------------------------------------------------------
	magnet_ACW()
	�������ת����ʱ�룬�����ر�
--------------------------------------------------------*/
void magnet_ACW(tWord x, y)
	{
	Externalmotor = 1;
	MagentControl_1 = 0;
	MagentControl_2 = 1;
	Magnet_Delay(x, y);
	MagentControl_1 = 1;
	MagentControl_2 = 1;
	}

/*-------------------------------------------------------
	magnet_ACW_Taili()
	�������ת����ʱ�룬�����ر�
--------------------------------------------------------*/
void magnet_ACW_Taili(tWord x, y)
	{
	Externalmotor = 1;
	MagentControl_1 = 0;
	MagentControl_2 = 1;
	Magnet_Delay(x, y);
	MagentControl_1 = 0;
	MagentControl_2 = 0;
	}
	
/*----------------------------------------------------
	disable_sensor()
	���񶯴����������ᴫ�����������临λ
----------------------------------------------------*/
void disable_sensor(void)
	{
	// disable position sensor(vertical and horizontal) and vibration sensor
	position_sensor_EN = 0;	
	sensor_EN = 0;
	// reset all the flag in sensor
	fell_flag=0;  
	raised_flag=0;
	sensor_trigger_count = 0;
	sensor_1ststage_count = 0;
	sensor_2ndstage_count = 0;
	sensor_2ndstage_time = 0;
	sensor_3rdstage_time = 0;
	sensor_3rdstage_interval = 0;
	sensor_3rdstage_count = 0;
	sensor_3rdstage_effcount = 0;
	stolen_alarm_flag = 0;
	host_stolen_alarm1_EN = 0;
	host_stolen_alarm1_count = 0;
	host_stolen_alarm2_EN = 0;
	host_stolen_alarm2_count = 0;
	
	raised_fell_flag = 0;
	
	// reset the wire broken count
	wire_broken_count = 0;
	wire_broken_flag = 0;
	
	enable_sensor_delayEN = 0;
	enable_sensor_delay_count = 0;
	// reset battery stolen
	battery_stolen_EN = 0;
	battery_stolen_count = 0;	
	}
	
/*--------------------------------------------------------
	enable_sensor()
	�򿪴�������ʹ������
---------------------------------------------------------*/
void enable_sensor(void)
	{
	// enable position sensor(vertical and horizontal) and vibration sensor
	position_sensor_EN = 1;	
	sensor_EN = 1;
	// reset relatively sensor count
	sensor_trigger_count = 0;
	sensor_1ststage_count = 0;	
	enable_sensor_delayEN = 0;
	enable_sensor_delay_count = 0;	
	}

/*----------------------------------------------------------------
	Moto_Vibration()
	���ڸ����������
-----------------------------------------------------------------*/
void Moto_Vibration(void)
	{
	Moto_EN=0;				
	Delay_500ms();
	Moto_EN=1;
	}
	
/*---------------------------------------------------
	end of file
----------------------------------------------------*/