/*---------------------------------------------------
	main.c (v1.00)

	DDC-Z program, for electrocar.
----------------------------------------------------*/

//
#include "Main.h"
#include "port.h"

#include "AD.h"
#include "voice.h"
#include "pwm.h"
#include "Timer.h"
#include "Delay.h"
#include "communication.h"
#include "Battery.h"
#include "Other.h"
#include "operation.h"
#include "UART.h"
#include "ISP_DataFlash.h"
#include "schedular.h"
#include "ElecMotor.h"


/*------- Public variable declarations --------------------------*/
extern tByte timer0_8H, timer0_8L, timer1_8H, timer1_8L;
extern bit enable_sensor_delayEN;
extern bit sensor_EN;

/*------- Public variable definitions --------------------------*/
bit ID_certificated_flag = 0;			// Flagged once ID card or Slave certificated successfully.
tByte ID_certificated_numbers = 0;	// Numbers of ID certificated.

bit Host_stolen_alarming = 0;			// Host stolen is alarming, don't detect vibration for 1st stage.

// ------ Private variable definitions -----------------------------
bit position_sensor_EN=0;  		//Î»ÖÃ´«¸ĞÆ÷£¬¼´µ¹µØÌ§Æğ´«¸ĞÆ÷×Ü¿ª¹Ø£¬1µÄÊ±ºò£¬¼ì²âÕâÁ½¸ö´«¸ĞÆ÷
bit EN_host_stolen_alarming = 0;      //ÅĞ¶ÏÎª±»µÁ±¨¾¯ºóµÄµÚÒ»¶ÎÓïÒôÊ¹ÄÜ
bit host_stolen_alarm2_EN = 0;      //ÅĞ¶ÏÎª±»µÁ±¨¾¯ºóµÄµÚ¶ş¶ÎÓïÒôÊ¹ÄÜ
tByte host_stolen_alarm1_count = 0;		//ÅĞ¶ÏÎª±»µÁ±¨¾¯ºóµÄµÚÒ»¶ÎÓïÒô´ÎÊı
tByte host_stolen_alarm2_count = 0;		//ÅĞ¶ÏÎª±»µÁ±¨¾¯ºóµÄµÚ¶ş¶ÎÓïÒô´ÎÊı
bit fell_flag=0;						//ÅĞ¶ÏÖ÷»úµ¹ÏÂºó£¬ÖÃ1
bit raised_flag=0;					//ÅĞ¶ÏÖ÷»ú±»Ì§Æğºó£¬ÖÃ1
tByte sensor_2ndstage_count = 0;		//´«¸ĞÆ÷½øÈëµÚ¶ş½×¶Î¼ì²âÊ±£¬¼ÆËãµÍµçÆ½µÄÊ±¼ä
tWord sensor_3rdstage_time = 0;			//´«¸ĞÆ÷½øÈëµÚÈı½×¶ÎµÄÊ±¼ä£¬
tByte sensor_3rdstage_effcount = 0;		//´«¸ĞÆ÷½øÈëµÚÈı½×¶Îºó£¬ÓĞĞ§´¥Åö´ÎÊıµÄ¼ÆÊı
tByte sensor_3rdstage_count = 0;			//´«¸ĞÆ÷½øÈëµÚÈı½×¶Îºó£¬µÍµçÆ½µÄ¼ÆÊı
tWord sensor_3rdstage_interval = 0;		//´«¸ĞÆ÷ÔÚµÚÈı½×¶ÎÖĞ£¬Ã¿´ÎÓĞĞ§µÍµçÆ½¼ÆÊıÖ®¼äµÄÊ±¼ä¼ä¸ô¡£ÔÚÕâÆÚ¼äµÄµÍµçÆ½²»ÈÏÎªÓĞĞ§¡£
bit raised_fell_flag = 0;					//µ¹µØ»òÕßÌ§Æğ´¥·¢ºó£¬´Ë±êÖ¾Î»ÖÃ1
tByte sensor_trigger_count=0;		//´«¸ĞÆ÷´¥·¢¼ÆÊı
tWord sensor_2ndstage_time=0;		//´«¸ĞÆ÷½øÈëµÚ¶ş½×¶ÎºóÁ÷ÊÅÊ±¼äµÄ¼ÆÊı
tByte sensor_1ststage_count=0;	//´«¸ĞÆ÷µÚÒ»½×¶ÎÅĞ¶ÏµÍµçÆ½µÄ¼ÆÊı

tByte raised_alarm_count = 0;    //Ö÷»ú±»Ì§Æğºó£¬Ïò¸½»ú·¢³ö±¨¾¯ĞÅºÅµÄ´ÎÊı
tByte fell_alarm_count = 0;        //Ö÷»úµ¹µØºó£¬Ïò¸½»ú·¢³ö±¨¾¯ĞÅºÅµÄ´ÎÊı
tWord timer0_count=0;		// counter for timer0, increment every ticket 			
tByte received_data_buffer[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};		//½ÓÊÕÊı¾İ»º´æ
bit receive_data_finished_flag = 0;		//½ÓÊÕÕâÒ»´®Êı¾İÍê³Éºó£¬´Ë±êÖ¾Î»ÖÃ1
tByte data_count = 0;				//½ÓÊÕÊı¾İ»º´æµÄÎ»Êı£¬¼´±íÃ÷µÚ¼¸¸ö»º´æÊı¾İ
tByte one_receive_byte = 0;		//½ÓÊÕÊı¾İµÄÒ»¸ö×Ö½Ú£¬½ÓÊÕÍêºó½«Æä¸³Öµ¸øreceived_data_bufferÏà¶ÔÓ¦µÄ×Ö½Ú
tByte one_receive_byte_count = 0;			//one_receive_byteÓĞ8Î»£¬´Ë¼ÆÊı±íÃ÷½ÓÊÕµ½µÚ¼¸Î»£¬Ã¿´Î¼ÆÊıµ½8µÄÊ±ºò±íÃ÷Ò»¸ö×Ö½Ú½ÓÊÕÍê³É¡£
bit receive_wire_flag = 1;		//½ÓÊÕÍ¨ĞÅÏßµÄ±êÖ¾Î»£¬1±íÃ÷¸ßµçÆ½£¬0±íÃ÷µÍµçÆ½£¬Ã¿´Îtimer1Òç³öÊ±£¬ÅĞ¶ÏP1.1Ò»´Î¡£ÒÔ´ËÀ´±íÃ÷ÊÇ·ñÎªÒ»´ÎÕıÈ·µÄÏÂ½µÑØ
tByte receive_HV_count = 0;		//¶¨Ê±Æ÷T1ÔÚÃ»ÓĞĞÅºÅµ½À´µÄÊ±ºò£¬¶Ô¸ßµçÆ½¼ÆÊı£¬Ò»µ©³¬¹ıÄ³¸öÖµ£¬Ôò½«one_receive_byte_countÇå0
tByte receive_LV_count = 0;		//Ã¿´Îtimer1Òç³öÊ±ÅĞ¶Ï½ÓÊÕÏßÈç¹ûÎªLV£¬Ôò¼ÆÊı¼Ó1£¬ÒÔ´ËÀ´±íÃ÷µÍµçÆ½µÄÊ±¼ä
tByte fell_wire_time=0;         //µ¹µØ¼ì²âÏß£¬¼ì²âµÍµçÆ½µÄÊ±¼ä
tByte raise_wire_time=0;			//Ì§Æğ¼ì²âÏß£¬¼ì²âµÍµçÆ½µÄÊ±¼ä
tWord raised_fell_number = 0;				//µ¹µØ»òÕßÌ§Æğ³ö·¢ºó£¬¼ÆÊı£¬µ½´ïÒ»¶¨ÊıÖµºó£¬½«ÆäÓë±êÖ¾Î»Ò»ÆğÇåÁã¡£
bit raised_fell_once_flag = 0;			//raised_fell_flagÊÇ·ñÔø¾­±êÖ¾¹ı£¬Èç¹û±êÖ¾¹ıÔòÖÃ1.È»ºóÖ÷»ú±»»Ö¸´µ¹µØ»òÕß»Ö¸´Ì§ÆğÊ±£¬´Ë±êÖ¾Î»¸´Î»¡£
tByte Open_action_flag = 0;			//µç¶¯³µ¿ªÆô¹Ø±Õ±êÖ¾Î»£¬1±íÊ¾µç¶¯³µ¿ªÆôÁË£¬0±íÊ¾µç¶¯³µ¹Ø±ÕÁË
tWord ADC_check_result = 0;		//×÷ÎªAD¼ì²âÖµ


tWord load_battery_result = 0xfff;
tByte wire_broken_count = 0;		// ×÷Îª¶ÏÏßºóµÄÊ±¼ä¼ì²â
bit battery_stolen_EN = 0;			// ×÷Îªµç³Ø±»µÁµÄÊ¹ÄÜ¶Ë
tByte battery_stolen_count = 0;	// ×÷Îªµç³Ø±»µÁµÄ±¨¾¯´ÎÊı
bit horizontal_vibration = 0;		// ±íÊ¾´¹Ö±´«¸ĞÆ÷ÔÚÕñ¶¯£¬´ËÊ±¾ÍËã¹ØÔ¿³×£¬Ò²²»ÄÜÖ´ĞĞ¹ØÔ¿³×µÄ²Ù×÷¡
tWord horizontal_vibration_count = 0;	//´¹Ö±´«¸ĞÆ÷´¥·¢ºó£¬¶ÔÊ±¼ä½øĞĞ¼ÆÊı¡£
bit vibration_flag = 0;
tWord vibration_count = 0;
bit wire_broken_flag = 0;			// ¼ô¶ÏËøÏßµÄ±êÖ¾Î»
tByte After_IDcert_timecount = 0;		// ID¿¨ÈÏÖ¤Í¨¹ıºó£¬¼ÆÊ±1·ÖÖÓ£¬Ê¹Ô¿³×ÄÜ×ª¶¯¡£
tByte enable_sensor_delay_count = 0;		// ´«¸ĞÆ÷ÑÓ³ÙµÄÊ±¼ä
bit sensor_3rdalarm_flag = 0;
bit wheeled_flag = 0;
tWord wheeled_count = 0;
bit IDkey_speech_flash = 0;
bit Emergency_open_G = 0;

bit Speech_closed_G = 0;
tByte Speech_closed_time = 0;

// If it is a debug program, define IDkey6~11 to RAM
#ifdef Debug
	tByte IDkey6;
	tByte IDkey7;
	tByte IDkey8;
	tByte IDkey9;
	tByte IDkey10;
	tByte IDkey11;
// If it is a normal program, define IDkey6~11 to Flash ROM
#else
	code tByte IDkey6 _at_ 0x003000;
	code tByte IDkey7 _at_ 0x003001;
	code tByte IDkey8 _at_ 0x003002;
	code tByte IDkey9 _at_ 0x003003;
	code tByte IDkey10 _at_ 0x003004;
	code tByte IDkey11 _at_ 0x003005;
#endif

tWord IDkey_selflearn_HVcount = 0;
tWord IDkey_selflearn_LVcount = 0;
bit IDkey_selflearn_flag1 = 0;
bit IDkey_selflearn_flag2 = 0;
bit IDkey_selflearn_flag3 = 0;
bit IDkey_selflearn_flag4 = 0;
bit IDkey_selflearn_flag5 = 0;
bit ID_selflearning_flag = 0;
tByte ID_selflearning_timecount = 0;
bit IDkey_flash_EN = 0;

bit Silence_Flag = 0;
bit flashing_flag = 0;

tByte Check_Motobattery_count = 0;
bit Check_Motobattery_flag = 0;
bit CheckADC_flag = 0;

bit never_alarm = 0;
bit never_alarm_speech = 0;

bit slave_nearby_actioned_flag = 0;
tByte slave_nearby_count = 0;

bit ID_speeched_flag = 0;

tByte Stolen_alarm_reset_count = 0;

bit wire_broken_reset = 0;

bit Just_power_up = 1;

bit Battery_hint_flag = 0;

bit Autolock_G = 0;
tWord timer0_count2 = 0;

/*------- Private variable declaratuions --------------------------*/

void main()
	{
	InitVoice();

	// lock the external motor, prohibit motor moving when power up.
//	InitElecmotor();

	ID_speech();

	InitUART(BAUD9600);

	InitSensor();

	InitTransceiver();
	
	
	// ½«P0.1, P0.2ÉèÖÃ³ÉÊäÈë¸ß×èÄ£Ê½
	P0M1 |= 0x06;
	P0M2 &= 0xf9;
	// ½«P2.5£¬¼´PIN16ÉèÖÃ³ÉÊäÈë¸ß×èÄ£Ê½
	P2M1 |= 0x20;
	P2M2 &= 0xdf;

	Lock_EN = 1;
	
	// µç»úËø
	Generator_lock = 0;
	Externalmotor = 1;

	lock_power = 0;
		
	while(1)
		{
		}
	}

/*------------------------------------------------------------------
	timerT0()
	operation every ticket.
--------------------------------------------------------------------*/
void timer0() interrupt interrupt_timer_0_overflow
	{
	// manually reload timer0 configuration
	TH0 = timer0_8H;
	TL0 = timer0_8L;

	// timer0 is 1ms ticket, count the time flow of timer0, then operate every 2s.
	if(++timer0_count >= 2000)
		{
		// reset timer0 ticket counter every 2s
		timer0_count=0;


//		UART_Send_Data_match();

		#ifdef ID
		if((++timer0_count2 >= 35)&&(Autolock_G == 0))
			{
			Autolock_G = 1;
			}
		#endif
/*----- Wire_cut detection ----------------------------------------*/
		if(sensor_EN == 1)
			{
			// judge the wire broken, if yes, it means someone has cut the wire of magnet lock
			if(wire_broken == 0)
				{
				EN_host_stolen_alarming = 1;
				host_stolen_alarm2_EN = 1;
				Stolen_alarm_reset_count = 0;		
				wire_broken_flag = 1;
				}
			else if((wire_broken == 1)&&(wire_broken_flag == 1))
				{
				wire_broken_reset = 1;
				wire_broken_flag = 0;				
				ID_speech();
				}
			}
		
		if(Speech_closed_G == 1)
			{
			Speech_closed_time += 1;
			if(Speech_closed_time > 2)
				{
				Speech_closed_time = 0;
				voice_EN = 0;
				}
			}

/*----- Accumulator relevantly ------------------------------------*/
		Check_motor_accumulator();
		Accumulator_voice_promot();

/*----- Enable sensor ---------------------------------------------*/
		#ifdef ID
		ENsensor_afterIDcert();
		#endif
		// if no vibration and wheeled, decrease slave_nearby_count,
		// if more than 3 times, it means slave is away, then enable sensor.
		#ifdef WX
		Ensensor_after_slave_away();
		#endif		

		ENsensor_After_Close();

/*----- Alarm relevantly -----------------------------------------*/
		#ifdef Z3
		Fell_Alarm_to_Slave();
		Raise_Alarm_to_Slave();
		Batstolen_Alarm_to_Slave();
		#endif
		
		Host_stolen_action();

/*----- Reset flag and disabling sensor relevantly --------------*/

		Reset_after_wirebroken();
		Reset_after_stolen_alarming();
		SelfLearn_Reset();
		}

	// Voice hint for entering no guard mode,
	Enter_noguard_voice();

/*----- Detectiong relevantly -----------------------------------*/
	Detect_selflearn_action();
	Detect_vibration();
	Detect_wheel_moving();
	
	Disable_sensor_after_IDcert();

	Detect_open_action();
	Detect_close_action();	
	
	if(Battery_hint_flag == 1)
		{
		Battery_hint_flag = 0;
		Battery_hint();
		}

// judge host is fell or raised every 1ms?
//	if((raised_sensor_detect == 1)&&(fell_sensor_detect == 1))
//		{
		// judge vibration sensor is enable?
		if(sensor_EN == 1)	
			{			
			// sensor trigger status, 0, 1 or 2?
			switch(sensor_trigger_count)
				{
				// case 0, it means host is in waiting status. waiting for first touch.
				case 0:
					{					
					// judge host been touched and also not in vibration alarm
//					if((sensor_detect == 0)&&(Host_stolen_alarming == 0)&&(transmiter_EN == 1))		
//					if(((sensor_detect == 0)||(horizontal_sensor == 0)||(the3rd_sendor == 0))&&(Host_stolen_alarming == 0)&&(flashing_flag == 0)&&(transmiter_EN == 1))		
					if(((sensor_detect == 0)||(horizontal_sensor == 0)||(the3rd_sendor == 0))&&(Host_stolen_alarming == 0)&&(flashing_flag == 0))		
						{
						// judge LV is more than 2ms, if yes, it means a effective touch
						if(++sensor_1ststage_count >= 1)			
							{
							sensor_1ststage_count=0;
							
							
							sensor_2ndstage_time = 0;
							// sensor trigge status progress to case 1.
							sensor_trigger_count = 1;
							// alarm speech for first touch
//							SCH_Add_Task(host_touch_speech, 0, 0);

							// æŠ¥è­¦æ—¶ä½¿æ§åˆ¶å™¨ä¾›ç”µ
							Generator_lock = 1;
							// ç”µæœºé”æ­»
							Externalmotor = 0;

							#ifdef voice
							host_touch_speech();
							#endif
							
							Delay_500ms();Delay_500ms();Delay_500ms();Delay_500ms();
                     }
						}
					else
						{
						// if no LV, reset the count.
						sensor_1ststage_count = 0;
						}
					}
				break;
				
				// waiting for next touch, 
				case 1:
					{
					if((sensor_detect == 0)||(horizontal_sensor == 0)||(the3rd_sendor == 0))
						{
						// LV for 2s, means a effective touch
						if(++sensor_2ndstage_count >= 1)
							{
							sensor_2ndstage_count = 0;
							sensor_trigger_count = 2;
							
							sensor_3rdstage_time = 0;
							// alarm speech for 2nd touch
							
							// æŠ¥è­¦æ—¶ä½¿æ§åˆ¶å™¨ä¾›ç”µ
							Generator_lock = 1;
							// ç”µæœºé”æ­»
							Externalmotor = 0;
								
							#ifdef voice
							//host_2ndtouch_speech();
							host_touch_speech();
							#endif
							}
						}
					else
						{
						sensor_2ndstage_count = 0;
						}

					// if there is no touch in 4s, reset sensor trigger status, etc.
					if(++sensor_2ndstage_time >= 8000)
						{
						sensor_trigger_count = 0;
						sensor_2ndstage_count = 0;
						sensor_1ststage_count = 0;
						sensor_2ndstage_time = 0;

						// æŠ¥è­¦æ—¶ä½¿æ§åˆ¶å™¨ä¾›ç”µ
						Generator_lock = 0;
						// ç”µæœºé”æ­»
						Externalmotor = 1;
						}
					}
				break;

				case 2:
					{
					if((sensor_detect == 0)||(horizontal_sensor == 0)||(the3rd_sendor == 0))
						{
						// LV for 2s, means a effective touch
						if(++sensor_3rdstage_count >= 2)
							{
							sensor_3rdstage_count = 0;
							sensor_trigger_count = 3;
							sensor_3rdstage_time = 0;

							// alarm speech for 2nd touch
//							host_2ndtouch_speech();

							// æŠ¥è­¦æ—¶ä½¿æ§åˆ¶å™¨ä¾›ç”µ
							Generator_lock = 1;
							// ç”µæœºé”æ­»
							Externalmotor = 0;

							#ifdef voice
							host_2ndtouch_speech();
							#endif
							}
						}
					else
						{
						sensor_3rdstage_count = 0;
						}
						
					// if there is no touch in 4s, reset sensor trigger status, etc.
					if(++sensor_3rdstage_time >= 8000)
						{
						sensor_trigger_count = 0;
						sensor_3rdstage_count = 0;
						sensor_2ndstage_count = 0;
						sensor_1ststage_count = 0;
						sensor_3rdstage_time = 0;

						// æŠ¥è­¦æ—¶ä½¿æ§åˆ¶å™¨ä¾›ç”µ
						Generator_lock = 0;
						// ç”µæœºé”æ­»
						Externalmotor = 1;
						}
					}
				break;
				
				// waiting for 3rd touch
				case 3:
					{
					if((sensor_detect == 0)||(horizontal_sensor == 0)||(the3rd_sendor == 0))
						{
						// 2s LV is a effective touch
						if(++sensor_3rdstage_count >= 3)
							{
							sensor_3rdstage_count = 0;
							// stolen alarm speech enable
							EN_host_stolen_alarming = 1;
							host_stolen_alarm2_EN = 1;	
							sensor_3rdalarm_flag = 1;	
							Stolen_alarm_reset_count = 0;
							
							// æŠ¥è­¦æ—¶ä½¿æ§åˆ¶å™¨ä¾›ç”µ
							Generator_lock = 1;
							// ç”µæœºé”æ­»
							Externalmotor = 0;
							}
						}
					else
						{
						sensor_3rdstage_count = 0;
						}
					
					// if there is no touch in 4s, reset all.
					if(++sensor_3rdstage_time >= 8000)
						{
						sensor_trigger_count = 0;
						sensor_1ststage_count = 0;
						sensor_2ndstage_count = 0;
						sensor_2ndstage_time = 0;
						sensor_3rdstage_time = 0;
						sensor_3rdstage_count = 0;
						
						if(EN_host_stolen_alarming == 0)
							{
							// æŠ¥è­¦æ—¶ä½¿æ§åˆ¶å™¨ä¾›ç”µ
							Generator_lock = 0;
							// ç”µæœºé”æ­»
							Externalmotor = 1;							
							}
						}
					}
				break;
				}
			
			
			if(ADC_check_result < 0x100)
				{
				battery_stolen_EN = 1;
				}
			else
				battery_stolen_EN = 0;
				}
//			}
	
	// judge whether position sensor is enable
	if(position_sensor_EN==1)		
		{
		// judge whether there is a LV
		if(raised_sensor_detect == 0)	
			{
			// LV > 0.5s means a effective input
			if(++raise_wire_time==10)
				{
				// flag raised, and reset fell
				raised_flag=1;
				fell_flag=0;
				// judge whether there once been a raised or fell.
				if(raised_fell_once_flag == 0)
					{
					// if no, flag raised and fell flag
					raised_fell_flag = 1;					
					}
				}		
			}
		else
			{
			raised_flag=0;
			raised_alarm_count=0;
			raise_wire_time=0;
			raised_fell_flag = 0;
			}
      // judge whether there is a LV
		if(fell_sensor_detect==0)
			{
			// LV > 0.5s means a effective input
			if(++fell_wire_time==10)	
				{ 
				//  flag fell, and reset raised
				fell_flag=1;			
				raised_flag=0;
				// judge whether there once been a raised or fell
				if(raised_fell_once_flag == 0)
					{
					raised_fell_flag = 1;					
					}
				}		
			}
		else
			{
			fell_flag=0;
			fell_alarm_count=0;
			fell_wire_time=0;
			raised_fell_flag = 0;
			}
		}
	
	// judge whether raised or fell, if yes, reset all after 10s.
	if(raised_fell_flag == 1)
		{
		if(++raised_fell_number >= 4000)
			{
			raised_fell_flag = 0;
			raised_fell_number = 0;
			raised_fell_once_flag = 1;
			
			sensor_trigger_count = 0;
			sensor_1ststage_count = 0;
			sensor_2ndstage_count = 0;
			sensor_2ndstage_time = 0;
			sensor_3rdstage_time = 0;
			sensor_3rdstage_interval = 800;
			sensor_3rdstage_count = 0;
			sensor_3rdstage_effcount = 0;					
			}
		}
 	}

/*-----------------------------------------------
	UART interrupt
-----------------------------------------------*/
void uart_isr() interrupt 4 
	{
	if(RI)
		{
		RI=0;
		received_data_buffer[data_count] = SBUF;

		// assign one byte to buffer[i] 
		
		if(ID_selflearning_flag == 0)
			{
			// judge whether buffer[0] is CmdHead
			if((data_count == 0) && (received_data_buffer[0] == IDkey6))
				{
				data_count = 1;
				}
			else if((data_count == 1) && (received_data_buffer[1] == IDkey7))
				{
				data_count = 2;
				}
			else if((data_count == 2) && (received_data_buffer[2] == IDkey8))
				{
				data_count = 3;
				}
			else if((data_count == 3) && (received_data_buffer[3] == IDkey9))
				{
				data_count = 4;
				}
			else if((data_count == 4) && (received_data_buffer[4] == IDkey10))
				{
				data_count = 5;
				}
			#ifdef ID
			else if((data_count == 5) && (received_data_buffer[5] == IDkey11))
				{
				data_count = 0;
				ID_certificated_flag = 1;
				After_IDcert_timecount = 0;
				IDkey_speech_flash = 1;
				
				IDcerted_speech();
						
				if(ID_certificated_numbers++ >= 1)
					{
					// é™éŸ³æ¨¡å¼
					// Silence_Flag = 1;
					}
				if(++ID_certificated_numbers >= 11)
					{
					//never_alarm = 1;
					
					// Ó¦¼±¿ªËø
					Emergency_open_G = ~Emergency_open_G;
					ID_certificated_flag = 1;
					never_alarm_speech = 1;
					
					Silence_Flag = 0;
					ID_certificated_numbers = 0;
					}
				}
			#endif
			#ifdef WX
			else if(data_count == 5)
				{
				receive_data_finished_flag = 1;
				data_count = 0;
				}			
			#endif
			else
				{
				data_count = 0;
				}

			#ifdef WX
			if(receive_data_finished_flag == 1)
				{
				receive_data_finished_flag  = 0;
				switch(received_data_buffer[5])
					{
					case ComMode_1:
						{
						ID_certificated_flag = 1;
						After_IDcert_timecount = 0;
						IDkey_speech_flash = 1;
						IDcerted_speech();
						slave_nearby_count = 0;
						}
					break;

					case ComMode_11:
						{
						Silence_Flag = 1;
						Self_learn_speech();
						}
					break;

					case ComMode_12:
						{
						Battery_hint_flag = 1;
						}
					break;
					}
				}
			#endif
			}
		else
			{
			#ifdef ID
			if(++data_count >= 6)
				{
				data_count = 0;
				IDkey_flash_EN = 1;
				}
			#endif

			#ifdef WX
			if((data_count == 0)&&(received_data_buffer[0] == CmdHead))
				{
				data_count = 1;
				}
			else if((data_count == 1)&&(received_data_buffer[1] == ComMode_1))
				{
				data_count = 2;
				}
			else if(data_count == 2)
				{
				data_count = 3;
				}
			else if(data_count == 3)
				{
				data_count = 4;
				}
			else if(data_count == 4)
				{
				data_count = 5;
				}
			else if(data_count == 5)
				{
				data_count = 6;
				}
			else if(data_count == 6)
				{
				data_count = 0;
				IDkey_flash_EN = 1;
				ID_speech();
				}
			else
				{
				data_count = 0;
				}
			#endif
			}
		}
	}
/*---------------------------------------------------
	end of file
----------------------------------------------------*/