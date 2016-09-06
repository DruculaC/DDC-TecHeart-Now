/*---------------------------------------------------
	main.c (v1.00)
	main-201604
	DDC-F program, for electrocar.
----------------------------------------------------*/

#include "main.h"             
#include "port.h"

#include "AD.h"
#include "Timer.h"
#include "voice.h"
#include "pwm.h"
#include "Delay.h"
#include "communication.h"
#include "Other.h"
#include "battery.h"
#include "UART.h"
#include "ISP_DataFlash.h"
#include "schedular.h"


/*------- Public variable declarations --------------------------*/
extern tByte timer0_8H, timer0_8L, timer1_8H, timer1_8L;
extern bit enable_sensor_delayEN;

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
tByte nearby_away_interval = 0;		//¸½»úÀë¿ªºÍ¿¿½üÊ±£¬ÓïÒôÌáÊ¾ºÍ¿ª¹ØËøµÄÊ±¼ä¼ä¸ô

tByte raised_alarm_count = 0;    //Ö÷»ú±»Ì§Æðºó£¬Ïò¸½»ú·¢³ö±¨¾¯ÐÅºÅµÄ´ÎÊý
tByte fell_alarm_count=0;        //Ö÷»úµ¹µØºó£¬Ïò¸½»ú·¢³ö±¨¾¯ÐÅºÅµÄ´ÎÊý
tWord timer0_count=0;		// counter for timer0, increment every ticket 			
tByte received_data_buffer[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};		//½ÓÊÕÊý¾Ý»º´æ
bit receive_data_finished_flag = 0;		//½ÓÊÕÕâÒ»´®Êý¾ÝÍê³Éºó£¬´Ë±êÖ¾Î»ÖÃ1
tByte data_count = 0;				//½ÓÊÕÊý¾Ý»º´æµÄÎ»Êý£¬¼´±íÃ÷µÚ¼¸¸ö»º´æÊý¾Ý
tByte one_receive_byte = 0;		//½ÓÊÕÊý¾ÝµÄÒ»¸ö×Ö½Ú£¬½ÓÊÕÍêºó½«Æä¸³Öµ¸øreceived_data_bufferÏà¶ÔÓ¦µÄ×Ö½Ú
tByte one_receive_byte_count = 0;			//one_receive_byteÓÐ8Î»£¬´Ë¼ÆÊý±íÃ÷½ÓÊÕµ½µÚ¼¸Î»£¬Ã¿´Î¼ÆÊýµ½8µÄÊ±ºò±íÃ÷Ò»¸ö×Ö½Ú½ÓÊÕÍê³É¡£
bit receive_wire_flag = 1;			//½ÓÊÕÍ¨ÐÅÏßµÄ±êÖ¾Î»£¬1±íÃ÷¸ßµçÆ½£¬0±íÃ÷µÍµçÆ½£¬Ã¿´Îtimer1Òç³öÊ±£¬ÅÐ¶ÏP1.1Ò»´Î¡£ÒÔ´ËÀ´±íÃ÷ÊÇ·ñÎªÒ»´ÎÕýÈ·µÄÏÂ½µÑØ
tByte receive_HV_count = 0;		//¶¨Ê±Æ÷T1ÔÚÃ»ÓÐÐÅºÅµ½À´µÄÊ±ºò£¬¶Ô¸ßµçÆ½¼ÆÊý£¬Ò»µ©³¬¹ýÄ³¸öÖµ£¬Ôò½«one_receive_byte_countÇå0
tByte receive_LV_count = 0;		//Ã¿´Îtimer1Òç³öÊ±ÅÐ¶Ï½ÓÊÕÏßÈç¹ûÎªLV£¬Ôò¼ÆÊý¼Ó1£¬ÒÔ´ËÀ´±íÃ÷µÍµçÆ½µÄÊ±¼ä
tByte fell_wire_time=0;          //µ¹µØ¼ì²âÏß£¬¼ì²âµÍµçÆ½µÄÊ±¼ä
tByte raise_wire_time=0;			//Ì§Æð¼ì²âÏß£¬¼ì²âµÍµçÆ½µÄÊ±¼ä
tWord raised_fell_number = 0;				//µ¹µØ»òÕßÌ§Æð³ö·¢ºó£¬¼ÆÊý£¬µ½´ïÒ»¶¨ÊýÖµºó£¬½«ÆäÓë±êÖ¾Î»Ò»ÆðÇåÁã¡£
bit raised_fell_once_flag = 0;			//raised_fell_flagÊÇ·ñÔø¾­±êÖ¾¹ý£¬Èç¹û±êÖ¾¹ýÔòÖÃ1.È»ºóÖ÷»ú±»»Ö¸´µ¹µØ»òÕß»Ö¸´Ì§ÆðÊ±£¬´Ë±êÖ¾Î»¸´Î»¡£
tByte key_rotated_on_flag=0;			//µç¶¯³µ¿ªÆô¹Ø±Õ±êÖ¾Î»£¬1±íÊ¾µç¶¯³µ¿ªÆôÁË£¬0±íÊ¾µç¶¯³µ¹Ø±ÕÁË
tWord ADC_check_result = 0;		//×÷ÎªAD¼ì²âÖµ
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
tWord match_button_count = 0;
tWord Press_open_button_count = 0;

// ------ Private variable definitions -----------------------------
tWord stolen_alarm_count = 0;
bit raised_alarm_flag = 0;
bit fell_alarm_flag = 0;
bit Host_battery_high_flag = 0;
bit wire_broken_EN = 0;
tWord Mode_detect_count = 0;
bit Mode_select = 0;	// Ä£Ê½Ñ¡Ôñ£¬0±íÊ¾×Ô¶¯Ä£Ê½£¬1±íÊ¾ÊÖ¶¯Ä£Ê½

bit powersave_enable = 0;

bit itrpt_EN = 1;
tByte itrpt_count = 0;
bit idle_EN = 0;

bit Auto_transmit = 0;			// 0±íÊ¾·Ç×Ô¶¯·¢ËÍÊý¾Ý£¬1±íÊ¾×Ô¶¯·¢ËÍÊý¾Ý½øÐÐÈÏÖ¤
bit IDflash_EN = 0;
tWord match_button_HVcount = 0;
tWord match_button_LVcount = 0;
bit match_button_flag1 = 0;
bit match_button_flag2 = 0;
bit match_button_flag3 = 0;
bit match_button_flag4 = 0;
bit match_button_flag5 = 0;
bit match_button_flag6 = 0;
tByte match_button_flag6count = 0;

tWord receiver_EN_count = 0;
tWord receiver_DisEN_count = 0;

bit match_moto_EN = 0;

bit battery_LV_flag = 0;
tByte battery_LV_flag_count = 0;
bit battery_HV_flag = 0;
tByte battery_HV_flag_count = 0;
tByte battery_HV_speech_count = 0;
bit battery_HV_speech_over = 0;

tWord Toggle_button_HV_count = 0;
bit Toggle_button_HV_flag = 0;

bit battery_ULV_flag = 0;
tByte battery_ULV_count = 0;
bit battery_hint_action = 0;
bit vibration_send_EN = 0;

#ifdef WX
code tByte IDkey6 _at_ 0x001ffb;
code tByte IDkey7 _at_ 0x001ffc;
code tByte IDkey8 _at_ 0x001ffd;
code tByte IDkey9 _at_ 0x001ffe;
code tByte IDkey10 _at_ 0x001fff;
code tByte IDkey11 _at_ 0x001fff;
#endif
#ifdef ID
code tByte IDkey6 _at_ 0x003000;
code tByte IDkey7 _at_ 0x003001;
code tByte IDkey8 _at_ 0x003002;
code tByte IDkey9 _at_ 0x003003;
code tByte IDkey10 _at_ 0x003004;
code tByte IDkey11 _at_ 0x003005;
#endif

bit ID_selflearning_flag = 0;
tByte ID_selflearning_time = 0;
tByte match_button_level = 0;

extern tByte myTxRxData[7];

/*--------------------------------------------------------------*/

void main(void)
	{
	InitVoice();

	InitUART(BAUD1200);

	#ifdef F2
	InitUART(BAUD9600);
	#endif

	// ¼üÅÌÖÐ¶Ï³õÊ¼»¯
	press_open_button = 1;
	wakeup_button = 1;
	
	#ifdef WX
	KBLS1 |= 0x11;
	KBLS0 |= 0x11;
	KBIF &= 0xee;
	KBIE |= 0x11;
	EKB = 1;
	#endif

	EA = 1;

//	Moto_EN = 1;		//³õÊ¼»¯£¬¹Ø±ÕÂí´ï
	voice_EN = 0;		  	//¿ª»úÊ±£¬½«¹¦·Å¹Ø±Õ
	P10 = 1;
	
//	ADC_check_result = 0x3ff;
	
	stolen_alarm_count = 0;			//Çå±¨¾¯¼ÆÊýÆ÷
	stolen_alarm_flag = 0;			//Çå±¨¾¯±êÖ¾

//	transmit_wire = 0;
	transmiter_EN = 0;		// turn off the transmitter
	receiver_EN = 0;			// turn on the receiver
	transceiver_power_enable = 1;         // ÉÏµçÊ±ÎÞÏßÄ£¿éµçÔ´¹Ø±Õ


	while(1)
		{
		#ifdef Z2
		if(idle_EN == 1)
			{
			idle_EN = 0;
			PCON |= 0x02;			
			}
		#endif
			
		
		sEOS_Go_To_Sleep();			
		}
	}

/*--------------------------------------------------------------------
	timerT0()
	¶¨Ê±Æ÷0Ã¿´ÎÒç³öºóÖ´ÐÐµÄ²Ù×÷
--------------------------------------------------------------------*/

void timer0() interrupt interrupt_timer_0_overflow
	{
	// ÖØ×°ÔÚ¶¨Ê±Æ÷µÄÊ±¼äÉèÖÃ
	TH0 = timer0_8H;
	TL0 = timer0_8L;




	// ÉèÖÃÒ»¸öÃ¿3sµÄ²Ù×÷
	if(++timer0_count >= 2000)		
		{

		// Ö÷»ú±»µÁ±¨¾¯
		if(stolen_alarm_flag == 1)		
			{
			// ÓïÒôÌáÊ¾£¬Âí´ïÕñ¶¯
			Alarm_stolen_speech();		
			Moto_Vibration();          
			stolen_alarm_flag = 0;
			}

		// Ì§ÆðÐÅºÅ±¨¾¯£¬Ã¿´Î±¨ÍêºóÇå0£¬Èç¹ûÔÙ´Î½Óµ½Ôò¼ÌÐø±¨¡£Ò»°ãÀ´Ëµ£¬Ö÷»úÃ¿´ÎÌ§ÆðÖ»·¢4±é¡£
		if(raised_alarm_flag == 1)	
			{
			Alarm_raised_speech();		
			Moto_Vibration();         
			raised_alarm_flag = 0;
			}

		// µ¹µØÐÅºÅ±¨¾¯£¬Ã¿´Î±¨ÍêºóÇå0£¬Èç¹ûÔÙ´Î½Óµ½Ôò¼ÌÐø±¨¡£Ò»°ãÀ´Ëµ£¬Ö÷»úÃ¿´Îµ¹µØÖ»·¢4±é¡£
		if(fell_alarm_flag == 1)
			{
			Alarm_fell_speech();		  
			Moto_Vibration();         
			fell_alarm_flag=0;
			}

		if(wire_broken_EN == 1)
			{
			wire_broken_speech();
			Moto_Vibration();         
			wire_broken_EN = 0;
			}
		
		if(battery_stolen_EN == 1)
			{
         battery_stolen_speech_F3();
			battery_stolen_EN = 0;
			Moto_Vibration();         			
			}

		if(ID_selflearning_flag == 1)
			{
			ID_selflearning_time += 1;
			if(ID_selflearning_time > 10)
				{
				ID_selflearning_flag = 0;
				ID_selflearning_time = 0;
				}
			}
		
		
		// Ã¿¸ö3s×öÒ»´ÎµçÁ¿¼ì²â£¬²¢½øÐÐÏà¹ØµÄµçÁ¿ÌáÊ¾
		Check_motor_accumulator();

		#ifdef Z2
		if(ADC_check_result <= 0x36a)		// 2.96V/3.47V µçÁ¿·Ç³£²»×ã
			{
			battery_ULV_flag = 1;
			battery_LV_flag = 0;
			}
		else if((ADC_check_result > 0x36a)&&(ADC_check_result <= 0x375)) 	// 3.27V/3.78V µçÁ¿²»×ã
			{
			battery_ULV_flag = 0;
			battery_LV_flag = 1;
			}
		else if(ADC_check_result > 0x375)
			{
			battery_ULV_flag = 0;
			battery_LV_flag = 0;
			}
			
		if(battery_ULV_flag == 1)
			{
			if(++battery_ULV_count >= 1)
				{
				SC_RST = ~SC_RST;
				battery_ULV_count = 0;
				battery_hint_action = 1;
				}
			}
			
		if(battery_LV_flag == 1)
			{
			if(++battery_LV_flag_count > 6)
				{
				SC_RST = ~SC_RST;
				battery_LV_flag_count = 0;
				battery_hint_action = 1;
				}
			}
		#endif
		
		#ifdef Z3
		if(ADC_check_result <= 0x36a)                 // 3.11V/3.64V µçÁ¿²»×ã
			{
			battery_LV_flag = 1;
			battery_HV_flag = 0;
			battery_HV_speech_over = 0;
			}
		else if(ADC_check_result >= 0x37a)            // 3.52V/4.03V µçÁ¿³ä×ã
			{
			battery_HV_flag = 1;
			battery_LV_flag = 0;
			}
		else
			{
			battery_LV_flag = 0;
			battery_HV_flag = 0;
			}
				
		if(battery_LV_flag == 1)
			{
			if(++battery_LV_flag_count > 20)
				{
				Battery_low_alarm_speech();
				battery_LV_flag_count = 0;
				}			
			}
		#endif

		#ifdef WX
		if((wakeup_button == 1)&&(EKB == 0))
			{
			if(++Toggle_button_HV_count > 150)
				{
				transceiver_power_enable = 0;
				TXD = 0;
				MagentControl_2 = 1;
				vibration_send_EN = 0;
				
				EKB = 1;
				idle_EN = 1;
				}
			}
		#endif
		timer0_count = 0;
		}
	
	if(wakeup_button == 0)
		{
		Toggle_button_HV_flag = 0;
		Toggle_button_HV_count = 0;
		MagentControl_2 = 0;
		}

	if((transceiver_power_enable == 1)&&(ID_selflearning_flag == 0))
		{
		#ifdef Z2
		if(++receiver_EN_count > 80)
		#endif
		#ifdef Z3
		if(++receiver_EN_count > 300)
		#endif
			{
			receiver_EN_count = 0;
			
			if(battery_hint_action == 1)
				{
				UART_Send_Data_F(ComMode_12);			

				battery_hint_action = 0;
				SC_RST = ~SC_RST;
				}
			
			#ifdef F2
			if(vibration_send_EN == 1)
				{
				tByte i;
				
				UART_Send_Data_F(ComMode_1);				
				for(i = 0; i < 15; i++)
					UART_Send_Data_F2(ComMode_1);
				}
			#endif
			
			#ifdef F3
			if(vibration_send_EN == 1)
				{
				tByte i;
				
				open_tranceiver_F();				
				
				initsignal_F();	
				UART_Send_Data_F3(ComMode_1);
				for(i = 0; i < 15; i++)
					UART_Send_Data_F3(ComMode_1);
				
				close_tranceiver_F();				
				}
			#endif
			
			transceiver_power_enable = 0;
			RXD = 0;
			TXD = 0;			
			receiver_EN = 0;
			transmiter_EN = 0;
			}
		}	

	if((transceiver_power_enable == 0)&&(ID_selflearning_flag == 0))
		{		
		#ifdef Z2
		if(++receiver_DisEN_count > 4000)
		#endif
		#ifdef Z3
		if(++receiver_DisEN_count > 2000)
		#endif
			{
			transceiver_power_enable = 1;
			
			receiver_DisEN_count = 0;

			#ifdef F3
			transmiter_EN = 1;
			receiver_EN = 0;
			RXD = 1;
			#endif

			}
		}
	
	if(press_open_button == 0)
		{
		if(++Press_open_button_count > 3000)
			{
			transceiver_power_enable = 1;
			
			UART_Send_Data_F(ComMode_11);
			
			TXD = 0;
			transceiver_power_enable = 0;
			}
		}
	else
		{
		Press_open_button_count = 0;
		}
	
	if(match_button == 0)
		{
		match_button_LVcount += 1;
		if(match_button_LVcount >= 6000)
			{
			match_button_LVcount = 6001;
			match_button_level = 0;
			ID_selflearning_flag = 0;
			}
		}
	else
		{
		if(match_button_LVcount > 50)
			{
			match_button_LVcount = 0;
			match_button_level += 1;
			}
		}
	
	if(match_button_level >= 3)
		{
		match_button_level = 0;
		ID_selflearning_flag = 1;
		data_count = 0;
		
		Moto_Vibration();
		Delay(10);
		Moto_Vibration();
		
		transceiver_power_enable = 1;

		#ifdef ID
		receiver_EN = 0;
		RXD = 1;
		#endif
		
		#ifdef WX
		UART_Send_Data_match_F();
		#endif
		}

	#ifdef ID
	if(IDflash_EN == 1)
		{
		IDflash_EN = 0;

      Self_learn_programming_F();

		Moto_Vibration();
		Delay(10);
		Moto_Vibration();

		ID_selflearning_flag = 0;
		
		}
	#endif
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
				MagentControl_2 = ~MagentControl_2;
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
			else if(data_count == 5)
				{
				receive_data_finished_flag = 1;
				data_count = 0;
				}
			else 
				{
				data_count = 0;
				}

			if(receive_data_finished_flag==1)	
				{
				receive_data_finished_flag=0;			
				switch(received_data_buffer[5])
					{
					case ComMode_2:
						{
						battery_stolen_EN = 1;
						}
					break;
					
					case ComMode_3:
						{
						stolen_alarm_flag = 1;

						raised_alarm_count=0;
						raised_alarm_flag=0;
						fell_alarm_count=0;
						fell_alarm_flag=0;
						}
					break;
				
					case ComMode_4:
						{
						raised_alarm_flag=1;

						stolen_alarm_count=0;
						stolen_alarm_flag=0;
						fell_alarm_count=0;
						fell_alarm_flag=0;
						}
					break;

					case ComMode_5:
						{
						fell_alarm_flag=1;	

						stolen_alarm_count=0;
						stolen_alarm_flag=0;
						raised_alarm_count=0;
						raised_alarm_flag=0;
						}
					break;

					case ComMode_6:
						{
						wire_broken_EN = 1;
						}
					break;
					}
				}
			}
		else
			{
			if((data_count == 0) && (received_data_buffer[0] == CmdHead))
//			if(data_count == 0)
				{
				data_count = 1;
				}
			else if((data_count == 1) && (received_data_buffer[1] == ComMode_1))
//			else if(data_count == 1)
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
				IDflash_EN = 1;
				}
			else 
				{
				data_count = 0;
				}
			}		
		}
	}

/*-----------------------------------------------------------
	KBI_ISR()
	¼üÅÌÖÐ¶Ï£¬Ê¹Ð¾Æ¬´ÓÊ¡µçÄ£Ê½ÖÐ»½ÐÑ
-----------------------------------------------------------*/
#ifdef WX
void KBI_ISR(void) interrupt 7
	{
	EKB = 0;
	KBIF &= 0xee;
	transceiver_power_enable = 0;
	vibration_send_EN = 1;
	receiver_EN = 0;
	RXD = 1;
	Toggle_button_HV_count = 0;
	}
#endif		
/*---------------------------------------------------
	end of file
----------------------------------------------------*/