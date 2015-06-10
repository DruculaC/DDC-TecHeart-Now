/*-----------------------------------------------------
	voice.h (v1.00)
	
	header file for voice.c
-----------------------------------------------------*/

#ifndef _VOICE_H
#define _VOICE_H

//-----public constants-------------------------------

//-----public function prototype----------------------

void SC_Speech(unsigned char cnt);
void SC_Speech2(unsigned char cnt);
void InitVoice();
void key_rotate_on_speech(void);
void motorBAT_low_speech(void);
void key_rotate_off_speech(void);
void stolen_alarm_speech1(void);
void stolen_alarm_speech2(void);
void open_lock_speech(void);
void close_lock_speech(void);
void host_touch_speech(void);
void host_2ndtouch_speech(void);
void battery_stolen_speech(void);
void IDcerted_speech(void);
void Accumulator_voice_promot(void);
void Enter_noguard_voice();

// �����õ���������
void Product_Test_Voice(void);
void Cutting_Wire_Voice(void);

// �����õ�����
void Battery_low_alarm_speech(void);
void Battery_high_alarm_speech(void);
void Alarm_stolen_speech(void);
void Alarm_raised_speech(void);
void Alarm_fell_speech(void);
void Host_battery_high_alarm_speech(void);
void battery_stolen_speech_F3(void);
void wire_broken_speech(void);
void lock_rotated_on_speech(void);
void lock_rotated_off_speech(void);
void ID_speech(void);
void Self_learn_speech(void);



#endif
											
/*---------------------------------------------------
	end of file
----------------------------------------------------*/