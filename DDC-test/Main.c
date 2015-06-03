/*---------------------------------------------------
	main.c (v1.00)
	
	DDC-Z program, for electrocar.
----------------------------------------------------*/

//µç¶¯³µ
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


// ------ Private variable definitions -----------------------------
bit stolen_alarm_flag = 0;					// when host been touch 3 times, this flag 1 before alarm voice present, not to detect sensor for 1st voice alarm.
bit position_sensor_EN=0;  		//Î»ÖÃ´«¸ÐÆ÷£¬¼´µ¹µØÌ§Æð´«¸ÐÆ÷×Ü¿ª¹Ø£¬1µÄÊ±ºò£¬¼ì²âÕâÁ½¸ö´«¸ÐÆ÷
bit host_stolen_alarm1_EN = 0;      //ÅÐ¶ÏÎª±»µÁ±¨¾¯ºóµÄµÚÒ»¶ÎÓïÒôÊ¹ÄÜ
bit host_stolen_alarm2_EN = 0;      //ÅÐ¶ÏÎª±»µÁ±¨¾¯ºóµÄµÚ¶þ¶ÎÓïÒôÊ¹ÄÜ
tByte host_stolen_alarm1_count = 0;		//ÅÐ¶ÏÎª±»µÁ±¨¾¯ºóµÄµÚÒ»¶ÎÓïÒô´ÎÊý
tByte host_stolen_alarm2_count = 0;		//ÅÐ¶ÏÎª±»µÁ±¨¾¯ºóµÄµÚ¶þ¶ÎÓïÒô´ÎÊý
bit fell_flag=0;						//ÅÐ¶ÏÖ÷»úµ¹ÏÂºó£¬ÖÃ1
bit raised_flag=0;					//ÅÐ¶ÏÖ÷»ú±»Ì§Æðºó£¬ÖÃ1
tByte sensor_2ndstage_count = 0;		//´«¸ÐÆ÷½øÈëµÚ¶þ½×¶Î¼ì²âÊ±£¬¼ÆËãµÍµçÆ½µÄÊ±¼ä
tWord sensor_3rdstage_time = 0;			//´«¸ÐÆ÷½øÈëµÚÈý½×¶ÎµÄÊ±¼ä£¬
tByte sensor_3rdstage_effcount = 0;		//´«¸ÐÆ÷½øÈëµÚÈý½×¶Îºó£¬ÓÐÐ§´¥Åö´ÎÊýµÄ¼ÆÊý
tByte sensor_3rdstage_count = 0;			//´«¸ÐÆ÷½øÈëµÚÈý½×¶Îºó£¬µÍµçÆ½µÄ¼ÆÊý
tWord sensor_3rdstage_interval = 0;		//´«¸ÐÆ÷ÔÚµÚÈý½×¶ÎÖÐ£¬Ã¿´ÎÓÐÐ§µÍµçÆ½¼ÆÊýÖ®¼äµÄÊ±¼ä¼ä¸ô¡£ÔÚÕâÆÚ¼äµÄµÍµçÆ½²»ÈÏÎªÓÐÐ§¡£
bit raised_fell_flag = 0;					//µ¹µØ»òÕßÌ§Æð´¥·¢ºó£¬´Ë±êÖ¾Î»ÖÃ1
tByte sensor_trigger_count=0;		//´«¸ÐÆ÷´¥·¢¼ÆÊý
tWord sensor_2ndstage_time=0;		//´«¸ÐÆ÷½øÈëµÚ¶þ½×¶ÎºóÁ÷ÊÅÊ±¼äµÄ¼ÆÊý
tByte sensor_1ststage_count=0;	//´«¸ÐÆ÷µÚÒ»½×¶ÎÅÐ¶ÏµÍµçÆ½µÄ¼ÆÊý

tByte raised_alarm_count = 0;    //Ö÷»ú±»Ì§Æðºó£¬Ïò¸½»ú·¢³ö±¨¾¯ÐÅºÅµÄ´ÎÊý
tByte fell_alarm_count = 0;        //Ö÷»úµ¹µØºó£¬Ïò¸½»ú·¢³ö±¨¾¯ÐÅºÅµÄ´ÎÊý
tWord timer0_count=0;		// counter for timer0, increment every ticket 			
tByte received_data_buffer[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};		//½ÓÊÕÊý¾Ý»º´æ
bit receive_data_finished_flag = 0;		//½ÓÊÕÕâÒ»´®Êý¾ÝÍê³Éºó£¬´Ë±êÖ¾Î»ÖÃ1
tByte data_count = 0;				//½ÓÊÕÊý¾Ý»º´æµÄÎ»Êý£¬¼´±íÃ÷µÚ¼¸¸ö»º´æÊý¾Ý
tByte one_receive_byte = 0;		//½ÓÊÕÊý¾ÝµÄÒ»¸ö×Ö½Ú£¬½ÓÊÕÍêºó½«Æä¸³Öµ¸øreceived_data_bufferÏà¶ÔÓ¦µÄ×Ö½Ú
tByte one_receive_byte_count = 0;			//one_receive_byteÓÐ8Î»£¬´Ë¼ÆÊý±íÃ÷½ÓÊÕµ½µÚ¼¸Î»£¬Ã¿´Î¼ÆÊýµ½8µÄÊ±ºò±íÃ÷Ò»¸ö×Ö½Ú½ÓÊÕÍê³É¡£
bit receive_wire_flag = 1;		//½ÓÊÕÍ¨ÐÅÏßµÄ±êÖ¾Î»£¬1±íÃ÷¸ßµçÆ½£¬0±íÃ÷µÍµçÆ½£¬Ã¿´Îtimer1Òç³öÊ±£¬ÅÐ¶ÏP1.1Ò»´Î¡£ÒÔ´ËÀ´±íÃ÷ÊÇ·ñÎªÒ»´ÎÕýÈ·µÄÏÂ½µÑØ
tByte receive_HV_count = 0;		//¶¨Ê±Æ÷T1ÔÚÃ»ÓÐÐÅºÅµ½À´µÄÊ±ºò£¬¶Ô¸ßµçÆ½¼ÆÊý£¬Ò»µ©³¬¹ýÄ³¸öÖµ£¬Ôò½«one_receive_byte_countÇå0
tByte receive_LV_count = 0;		//Ã¿´Îtimer1Òç³öÊ±ÅÐ¶Ï½ÓÊÕÏßÈç¹ûÎªLV£¬Ôò¼ÆÊý¼Ó1£¬ÒÔ´ËÀ´±íÃ÷µÍµçÆ½µÄÊ±¼ä
tByte fell_wire_time=0;         //µ¹µØ¼ì²âÏß£¬¼ì²âµÍµçÆ½µÄÊ±¼ä
tByte raise_wire_time=0;			//Ì§Æð¼ì²âÏß£¬¼ì²âµÍµçÆ½µÄÊ±¼ä
tWord raised_fell_number = 0;				//µ¹µØ»òÕßÌ§Æð³ö·¢ºó£¬¼ÆÊý£¬µ½´ïÒ»¶¨ÊýÖµºó£¬½«ÆäÓë±êÖ¾Î»Ò»ÆðÇåÁã¡£
bit raised_fell_once_flag = 0;			//raised_fell_flagÊÇ·ñÔø¾­±êÖ¾¹ý£¬Èç¹û±êÖ¾¹ýÔòÖÃ1.È»ºóÖ÷»ú±»»Ö¸´µ¹µØ»òÕß»Ö¸´Ì§ÆðÊ±£¬´Ë±êÖ¾Î»¸´Î»¡£
tByte key_rotated_on_flag = 0;			//µç¶¯³µ¿ªÆô¹Ø±Õ±êÖ¾Î»£¬1±íÊ¾µç¶¯³µ¿ªÆôÁË£¬0±íÊ¾µç¶¯³µ¹Ø±ÕÁË
tWord ADC_check_result = 0;		//×÷ÎªAD¼ì²âÖµ
tWord load_battery_result = 0;
tByte wire_broken_count = 0;		// ×÷Îª¶ÏÏßºóµÄÊ±¼ä¼ì²â
bit battery_stolen_EN = 0;			// ×÷Îªµç³Ø±»µÁµÄÊ¹ÄÜ¶Ë
tByte battery_stolen_count = 0;	// ×÷Îªµç³Ø±»µÁµÄ±¨¾¯´ÎÊý
bit horizontal_vibration = 0;		// ±íÊ¾´¹Ö±´«¸ÐÆ÷ÔÚÕñ¶¯£¬´ËÊ±¾ÍËã¹ØÔ¿³×£¬Ò²²»ÄÜÖ´ÐÐ¹ØÔ¿³×µÄ²Ù×÷¡
tWord horizontal_vibration_count = 0;	//´¹Ö±´«¸ÐÆ÷´¥·¢ºó£¬¶ÔÊ±¼ä½øÐÐ¼ÆÊý¡£
bit vibration_flag = 0;
tWord vibration_count = 0;
bit wire_broken_flag = 0;			// ¼ô¶ÏËøÏßµÄ±êÖ¾Î»
bit IDkey_flag = 0;			// µ±ID¿¨¿¿½üÊ±ÈÏÖ¤Í¨¹ýºóÖÃ1£¬
tByte IDkey_count = 0;		// ID¿¨ÈÏÖ¤Í¨¹ýºó£¬¼ÆÊ±1·ÖÖÓ£¬Ê¹Ô¿³×ÄÜ×ª¶¯¡£
tByte enable_sensor_delay_count = 0;		// ´«¸ÐÆ÷ÑÓ³ÙµÄÊ±¼ä
bit sensor_3rdalarm_flag = 0;
bit wheeled_flag = 0;
tWord wheeled_count = 0;
bit IDkey_speech_flash = 0;

code tByte IDkey6 _at_ 0x003000;
code tByte IDkey7 _at_ 0x003001;
code tByte IDkey8 _at_ 0x003002;
code tByte IDkey9 _at_ 0x003003;
code tByte IDkey10 _at_ 0x003004;
code tByte IDkey11 _at_ 0x003005;

tWord IDkey_selflearn_HVcount = 0;
tWord IDkey_selflearn_LVcount = 0;
bit IDkey_selflearn_flag1 = 0;
bit IDkey_selflearn_flag2 = 0;
bit IDkey_selflearn_flag3 = 0;
bit IDkey_selflearn_flag4 = 0;
bit IDkey_selflearn_flag5 = 0;
bit IDkey_selflearn_flag6 = 0;
tByte IDkey_selflearn_flag6count = 0;
bit IDkey_flash_EN = 0;

tByte IDkey_certificated_times = 0;
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

/*------- Private variable declarations --------------------------*/

void main()
	{	
	InitVoice();
	
	#ifdef ID
	InitUART9600();
   #endif
	
	#ifdef WX
	InitUART600();
	#endif
	
	InitSensor();
	
	InitTransceiver();
	
	// lock the external motor, ·ÀÖ¹Ëø»¹Ã»ÍêÈ«´ò¿ªµÄÊ±ºò£¬³µÊÖ¼Óµçµ¼ÖÂÂÖ×ÓÓëËøµÄ¸æËßÅö×²¡£ 
	InitElecmotor();	
   
	Externalmotor = Close;
	
	// start Timer 0
	TR0 = 1;
	position_sensor_EN = 1;
	
	while(1)
		{
		hSCH_Dispatch_Tasks();
		if(fell_flag == 1)
			{
			Alarm_fell_speech();
			}	
		
		if(raised_flag == 1)		
			{
			Alarm_raised_speech();
			}			
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

	if(raised_sensor_detect == 0)
		{
		raised_flag = 1;
		fell_flag = 0;
		}
	else
		{
		raised_flag = 0;
		}

	if(fell_sensor_detect == 0)
		{
		fell_flag = 1;
		raised_flag = 0;
		}
	else
		{
		fell_flag = 0;
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
		
		if(IDkey_selflearn_flag6 == 0)
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
				IDkey_flag = 1;
				IDkey_count = 0;
				disable_sensor();
				IDkey_speech_flash = 1;
				
				if(IDkey_certificated_times++ >= 1)
					{
					Silence_Flag = 1;
					}
				if(++IDkey_certificated_times >= 11)
					{
					never_alarm = 1;
					never_alarm_speech = 1;
					Silence_Flag = 0;
					IDkey_certificated_times = 0;
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
						IDkey_flag = 1;
						IDkey_count = 0;
						disable_sensor();
						IDkey_speech_flash = 1;
						slave_nearby_count = 0;
						}
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
			}
		}
	}


/*---------------------------------------------------
	end of file
----------------------------------------------------*/