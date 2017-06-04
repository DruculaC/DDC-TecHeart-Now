/*---------------------------------------------------
	voice.c (v1.00)
	ͨ�������ļ�
	201603
---------------------------------------------------*/

#include "main.h"
#include "port.h"

#include "voice.h"
#include "Delay.h"
#include "communication.h"
#include "Other.h"
#include "battery.h"

/*------ private variable --------------------------*/
tByte key_rotate_on_speech_number = 1;		// ѭ�������ο�������
                                            
extern bit flashing_flag;
extern tByte Open_action_flag;		
extern bit slave_nearby_actioned_flag;
extern bit ID_speeched_flag;
extern tByte Check_Motobattery_count;
extern bit Check_Motobattery_flag;
extern bit EN_host_stolen_alarming;
extern tWord load_battery_result;
extern bit fell_flag;						
extern bit raised_flag;				
extern tWord ADC_check_result;		
extern bit battery_stolen_EN;			
extern bit never_alarm_speech;
extern bit Speech_closed_G;
extern tByte Speech_closed_time;


/*--------------------------------------------------
	SC_Speech()
	�������������壬���ɱ��ڼ���������
---------------------------------------------------*/
void SC_Speech(tByte cnt)
	{
	tByte ii;
	SC_RST = 1;
	Delay_50ms();
	SC_RST = 0;
	Delay_50ms();
	for(ii=0; ii < cnt; ii++)
		{
		SC_DATA = 1;
		delay_us(20);
		SC_DATA = 0;
		delay_us(20);
		}
	}

/*-------------------------------------------------
	InitVoice()
	Initialise Voice IC, shielding voice by accident on powerup.
--------------------------------------------------*/
void InitVoice()
	{
	P14=0;
	SC_DATA = 0;
	SC_RST = 1;
	delay_us(350); 
	SC_RST = 0;
	delay_us(350);
//	P14 = 1;	
	voice_EN = 0;				// Close speaker.
	}

/*----------------------------------------------------
	key_rotate_on_speech()
	Կ����ת��ON��������ʾ
-----------------------------------------------------*/
void key_rotate_on_speech(void)
	{
	#ifdef Taili
	voice_EN = 1;
	SC_Speech(26);  
	Delay(100);
	voice_EN = 0;
	#endif

	#ifdef Shengbaolong
	voice_EN = 1;
	SC_Speech(5);  
	Delay(30);
	SC_Speech(6);  
	Delay(30);
	SC_Speech(4);  
	Delay(100);	
	voice_EN = 0;	
	#endif
	}
	
/*-----------------------------------------------------
	key_rotate_off_speech()
	Կ����ת��OFF��������ʾ
void key_rotate_off_speech(void)
	{
	voice_EN = 1;
	SC_Speech(13);
	Delay(40);
	voice_EN = 0;
	}
------------------------------------------------------*/

/*----------------------------------------------------
	motorBAT_low_speech()
	������ƿ����������С�޶�ֵ��������ʾ�������㡣
-----------------------------------------------------*/
void motorBAT_low_speech(void)
	{
	voice_EN = 1;
	SC_Speech(4);
	Delay(80);
	voice_EN = 0;
	}

/*----------------------------------------------------
	Battery_hint()
	��һ��������������һ����ʾ����
--------------------------------------------------------*/
void Battery_hint(void)
	{
	voice_EN = 1;
	SC_Speech(24); 
	Delay(1);
	voice_EN = 0;
	}

/*----------------------------------------------------
	ID_speech()
	��һ��������������һ����ʾ����
--------------------------------------------------------*/
void ID_speech(void)
	{
	voice_EN = 1;
	SC_Speech(26); 
	Delay(30);
	voice_EN = 0;
//	Speech_closed_G = 1;
//	Speech_closed_time = 0;
	}

/*----------------------------------------------------
	Self_learn_speech()
	��һ��������������һ����ʾ����
--------------------------------------------------------*/
void Self_learn_speech(void)
	{
	voice_EN = 1;
	Speech_closed_time = 0;
	SC_Speech(27); 
	Delay(30);
	voice_EN = 0;
	flashing_flag = 0;
	}

/*----------------------------------------------------
	host_touch_speech()
	��һ��������������һ����ʾ����
--------------------------------------------------------*/
void host_touch_speech(void)
	{
	voice_EN = 1;
	SC_Speech(2);  
	Delay(90);
	voice_EN = 0;
	Delay(30);
	}

/*-----------------------------------------------------
	host_2ndtouch_speech()
	�ڶ��δ����������ٱ�һ����ʾ����
-------------------------------------------------------*/
void host_2ndtouch_speech(void)
	{
	voice_EN = 1;
	SC_Speech(22);  
	Delay(140);
	voice_EN = 0;
//	Delay(10);
	}
	
/*----------------------------------------------------
	stolen_alarm_speech()
	�ж�Ϊ������ʱ�򣬷�������������һ��
----------------------------------------------------*/
void stolen_alarm_speech1(void)
	{
	voice_EN = 1;
	SC_Speech(7); 
	Delay(60);
	voice_EN = 0; 
	}

/*----------------------------------------------------
	stolen_alarm_speech()
	�ж�Ϊ������ʱ�򣬷������������ڶ���
----------------------------------------------------*/
 void stolen_alarm_speech2(void)
	{
	voice_EN = 1;
	SC_Speech(3); 
	Delay(180);
	voice_EN = 0;
	Delay(20);
	}

/*----------------------------------------------------
	open_lock_speech()
	����������������ͨ����֤��������ʾ
-----------------------------------------------------*/
void open_lock_speech(void)
	{
	#ifdef Tailing
	voice_EN = 1;
	SC_Speech(24);  
	Delay(30);
	SC_Speech(6);  
	Delay(40);
	voice_EN = 0;
	#endif

	// �����Ѵ�
	voice_EN=1;
	Speech_closed_time = 0;
	SC_Speech(9); 
	Delay(60);
	voice_EN=0;
	}

/*----------------------------------------------------
	close_lock_speech()
	�жϸ����뿪��������ʾ
-----------------------------------------------------*/
void close_lock_speech(void)
	{
	voice_EN = 1;
	SC_Speech(10);  
	Delay(60);
	voice_EN = 0;
	}

/*----------------------------------------------------
	battery_stolen_speech()
	������⵽��ƽ����ʱ��������ʾ��ƽ����
-----------------------------------------------------*/
void battery_stolen_speech(void)
	{
	voice_EN = 1;
	SC_Speech(15);  
	Delay(100);
	voice_EN = 0;
	}

/*----------------------------------------------------
	Cutting_Wire_Voice()
	���˼������ߵ�ʱ�򣬲���������
-----------------------------------------------------*/
void Cutting_Wire_Voice(void)
	{
	voice_EN = 1;
	SC_Speech(28);  
	Delay(120);
	voice_EN = 0;
	}

/*----------------------------------------------------
	Product_Test_Voice()
	���Գ����е�������ʾ
-----------------------------------------------------*/
void Product_Test_Voice(void)
	{
	voice_EN = 1;
	SC_Speech(21);  
	Delay(150);
	voice_EN = 0;
	}

/*---------------------------------------------------------------
	Battery_low_alarm_speech()
	���ڸ���������ص�������һ��ֵ��������ʾ
----------------------------------------------------------------*/
void Battery_low_alarm_speech(void)
	{
	voice_EN = 1;
	SC_Speech(6);
	Delay(80);
	voice_EN = 0;
	}

/*---------------------------------------------------------------
	Battery_high_alarm_speech()
	���ڸ�������������һ��ֵ��ʱ��������ʾ
----------------------------------------------------------------*/
void Battery_high_alarm_speech(void)
	{
	voice_EN = 1;
	SC_Speech(5);
	Delay(50);
	voice_EN = 0;
	}

/*-------------------------------------------------------------
	Alarm_stolen_speech()
	���ڸ�����������������
---------------------------------------------------------------*/
void Alarm_stolen_speech(void)
	{
	voice_EN = 1;
	SC_Speech(4);
	Delay(180);
	voice_EN = 0;
	}
	
/*--------------------------------------------------------------
	Alarm_raised_speech()
	���ڸ�����������̧�𱨾�
---------------------------------------------------------------*/
void Alarm_raised_speech(void)
	{
	voice_EN = 1;
	SC_Speech(3);
	Delay(140);
	voice_EN = 0;
	}
	
/*--------------------------------------------------------------
	Alarm_fell_speech()
	���ڸ������������ر���
----------------------------------------------------------------*/
void Alarm_fell_speech(void)
	{
	voice_EN = 1;
	SC_Speech(2);
	Delay(120);
	voice_EN = 0;
	}
	
/*---------------------------------------------------------------
	Host_battery_high_alarm_speech()
	���ڸ�������������������ʾ
---------------------------------------------------------------*/
void Host_battery_high_alarm_speech(void)
	{
	voice_EN = 1;
	SC_Speech(11);  
	Delay(80);
	voice_EN = 0;
	}

/*---------------------------------------------------------------
	battery_stolen_speech_F3()
	���ڸ�����������ر�������
---------------------------------------------------------------*/
void battery_stolen_speech_F3(void)
	{
	voice_EN = 1;
	SC_Speech(7);  
	Delay(50);
	voice_EN = 0;
	}
	
/*---------------------------------------------------------------
	wire_broken_speech(void)
	���ڸ������������߱����ϱ���
----------------------------------------------------------------*/
void wire_broken_speech(void)
	{
	voice_EN = 1;
	SC_Speech(8);  
	Delay(80);
	voice_EN = 0;	
	}

/*---------------------------------------------------------------
	lock_rotated_on_speech(void)
	���ڸ������������߱����ϱ���
----------------------------------------------------------------*/
void lock_rotated_on_speech(void)
	{
	voice_EN = 1;
	SC_Speech(12);  
	Delay(80);
	voice_EN = 0;	
	}
/*---------------------------------------------------------------
	lock_rotated_off_speech(void)
	���ڸ������������߱����ϱ���
----------------------------------------------------------------*/
void lock_rotated_off_speech(void)
	{
	voice_EN = 1;
	SC_Speech(13);  
	Delay(80);
	voice_EN = 0;	
	}

/*-------------------------------------------------------------
	IDcerted_speech()
-------------------------------------------------------------*/
void IDcerted_speech(void)
	{
	#ifdef ID
	ID_speech();
	#endif
	
	#ifdef WX
	if((Open_action_flag == 0)&&(slave_nearby_actioned_flag == 0)&&(ID_speeched_flag == 0))
		{		
		ID_speech();
		
		ID_speeched_flag = 1;
		}
	#endif
	}
/*-------------------------------------------------------------
	Accumulator_voice_promot()
-------------------------------------------------------------*/	
void Accumulator_voice_promot(void)
	{
	if(++Check_Motobattery_count > 3)
		{
		Check_Motobattery_count = 5;
		if((Check_Motobattery_flag == 1)&&(EN_host_stolen_alarming == 0)&&(fell_flag == 0)&&(raised_flag == 0)&&(battery_stolen_EN == 0))
			{
			#ifdef BroadcastBattery
			Broadcast_battery();		
			#endif
			Check_Motobattery_flag = 0;
			}
		}
	}

/*--------------------------------------------------------
	Enter_noguard_voice()
--------------------------------------------------------*/
void Enter_noguard_voice()
	{
	if(never_alarm_speech == 1)
		{
		never_alarm_speech = 0;
		Self_learn_speech();					
		}	
	}
	
/*---------------------------------------------------
	end of file
----------------------------------------------------*/