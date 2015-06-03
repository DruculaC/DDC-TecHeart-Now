/*---------------------------------------------------
	main.c (v1.00)
	
	DDC-Z program, for electrocar.
----------------------------------------------------*/

//�綯��
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
bit position_sensor_EN=0;  		//λ�ô�������������̧�𴫸����ܿ��أ�1��ʱ�򣬼��������������
bit host_stolen_alarm1_EN = 0;      //�ж�Ϊ����������ĵ�һ������ʹ��
bit host_stolen_alarm2_EN = 0;      //�ж�Ϊ����������ĵڶ�������ʹ��
tByte host_stolen_alarm1_count = 0;		//�ж�Ϊ����������ĵ�һ����������
tByte host_stolen_alarm2_count = 0;		//�ж�Ϊ����������ĵڶ�����������
bit fell_flag=0;						//�ж��������º���1
bit raised_flag=0;					//�ж�������̧�����1
tByte sensor_2ndstage_count = 0;		//����������ڶ��׶μ��ʱ������͵�ƽ��ʱ��
tWord sensor_3rdstage_time = 0;			//��������������׶ε�ʱ�䣬
tByte sensor_3rdstage_effcount = 0;		//��������������׶κ���Ч���������ļ���
tByte sensor_3rdstage_count = 0;			//��������������׶κ󣬵͵�ƽ�ļ���
tWord sensor_3rdstage_interval = 0;		//�������ڵ����׶��У�ÿ����Ч�͵�ƽ����֮���ʱ�����������ڼ�ĵ͵�ƽ����Ϊ��Ч��
bit raised_fell_flag = 0;					//���ػ���̧�𴥷��󣬴˱�־λ��1
tByte sensor_trigger_count=0;		//��������������
tWord sensor_2ndstage_time=0;		//����������ڶ��׶κ�����ʱ��ļ���
tByte sensor_1ststage_count=0;	//��������һ�׶��жϵ͵�ƽ�ļ���

tByte raised_alarm_count = 0;    //������̧����򸽻����������źŵĴ���
tByte fell_alarm_count = 0;        //�������غ��򸽻����������źŵĴ���
tWord timer0_count=0;		// counter for timer0, increment every ticket 			
tByte received_data_buffer[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};		//�������ݻ���
bit receive_data_finished_flag = 0;		//������һ��������ɺ󣬴˱�־λ��1
tByte data_count = 0;				//�������ݻ����λ�����������ڼ�����������
tByte one_receive_byte = 0;		//�������ݵ�һ���ֽڣ���������丳ֵ��received_data_buffer���Ӧ���ֽ�
tByte one_receive_byte_count = 0;			//one_receive_byte��8λ���˼����������յ��ڼ�λ��ÿ�μ�����8��ʱ�����һ���ֽڽ�����ɡ�
bit receive_wire_flag = 1;		//����ͨ���ߵı�־λ��1�����ߵ�ƽ��0�����͵�ƽ��ÿ��timer1���ʱ���ж�P1.1һ�Ρ��Դ��������Ƿ�Ϊһ����ȷ���½���
tByte receive_HV_count = 0;		//��ʱ��T1��û���źŵ�����ʱ�򣬶Ըߵ�ƽ������һ������ĳ��ֵ����one_receive_byte_count��0
tByte receive_LV_count = 0;		//ÿ��timer1���ʱ�жϽ��������ΪLV���������1���Դ��������͵�ƽ��ʱ��
tByte fell_wire_time=0;         //���ؼ���ߣ����͵�ƽ��ʱ��
tByte raise_wire_time=0;			//̧�����ߣ����͵�ƽ��ʱ��
tWord raised_fell_number = 0;				//���ػ���̧������󣬼���������һ����ֵ�󣬽������־λһ�����㡣
bit raised_fell_once_flag = 0;			//raised_fell_flag�Ƿ�������־���������־������1.Ȼ���������ָ����ػ��߻ָ�̧��ʱ���˱�־λ��λ��
tByte key_rotated_on_flag = 0;			//�綯�������رձ�־λ��1��ʾ�綯�������ˣ�0��ʾ�綯���ر���
tWord ADC_check_result = 0;		//��ΪAD���ֵ
tWord load_battery_result = 0;
tByte wire_broken_count = 0;		// ��Ϊ���ߺ��ʱ����
bit battery_stolen_EN = 0;			// ��Ϊ��ر�����ʹ�ܶ�
tByte battery_stolen_count = 0;	// ��Ϊ��ر����ı�������
bit horizontal_vibration = 0;		// ��ʾ��ֱ���������񶯣���ʱ�����Կ�ף�Ҳ����ִ�й�Կ�׵Ĳ����
tWord horizontal_vibration_count = 0;	//��ֱ�����������󣬶�ʱ����м�����
bit vibration_flag = 0;
tWord vibration_count = 0;
bit wire_broken_flag = 0;			// �������ߵı�־λ
bit IDkey_flag = 0;			// ��ID������ʱ��֤ͨ������1��
tByte IDkey_count = 0;		// ID����֤ͨ���󣬼�ʱ1���ӣ�ʹԿ����ת����
tByte enable_sensor_delay_count = 0;		// �������ӳٵ�ʱ��
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
	
	// lock the external motor, ��ֹ����û��ȫ�򿪵�ʱ�򣬳��ּӵ絼�����������ĸ�����ײ�� 
	InitElecmotor();	
   
	Externalmotor = Close;
	
	// start Timer 0
	TR0 = 1;

	while(1)
		{
		Host_stolen_action();
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
		
		#ifdef ID
		ENsensor_afterIDcert();
		#endif
		
		CheckADC();
		
		ENsensor_After_CloseLock();
		
		SelfLearn_Reset();
		      
		#ifdef Batterycheck
		if(++Check_Motobattery_count > 3)
			{
			Check_Motobattery_count = 10;
			if(Check_Motobattery_flag == 1)
				{
				load_battery_result = ADC_check_result;
				verifybattery(load_battery_result);
				Check_Motobattery_flag = 0;
				}
			}
		#endif
			
		#ifdef WX
		if((vibration_flag == 0)&&(wheeled_flag == 0))
			{
			if(++slave_nearby_count > 3)
				{
				slave_nearby_count = 5;
				slave_nearby_actioned_flag = 0;
				IDkey_flag = 0;
				enable_sensor();
				}
			}

		#endif
		
		#ifdef Z3
		Fell_Alarm_to_Slave();
		Raise_Alarm_to_Slave();
		Batstolen_Alarm_to_Slave();
		#endif		
		}	

	IDcerted_speech();
	
	if(never_alarm_speech == 1)
		{
		never_alarm_speech = 0;
		Self_learn_speech();					
		}

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
				IDkey_selflearn_flag6 = 1;
				#ifdef WX
				IDkey_flash_EN = 1;
				#endif
				}
				
			if(++IDkey_selflearn_LVcount > 4000)
				{
				IDkey_selflearn_LVcount = 4002;
				IDkey_selflearn_flag1 = 0;
				IDkey_selflearn_flag2 = 0;
				IDkey_selflearn_flag3 = 0;
				IDkey_selflearn_flag4 = 0;
				IDkey_selflearn_flag5 = 0;
				IDkey_selflearn_flag6 = 0;
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
		IDkey_selflearn_flag6 = 0;
		#ifdef ID
		Self_learn_programming();
		#endif
		Self_learn_speech();
		#ifdef WX
		UART_Send_Data_match();
		#endif
		}
	
	// detect whether key is rotated on,  
	if((key_rotate == 1)&&(key_rotated_on_flag == 0)&&(IDkey_flag == 1)&&(never_alarm == 0))		
		{
		disable_sensor();
		key_rotated_on_flag = 1;
		ID_speeched_flag = 0;
		
		IDkey_count = 0;
		IDkey_flag = 0;
		IDkey_certificated_times = 0;
		slave_nearby_actioned_flag = 1;
		ElecMotor_CW();
		slave_nearby_operation();
		} 		
				
	// detect whether key is rotated off
	if(((key_rotate == 0)||(slave_nearby_actioned_flag == 0))&&(key_rotated_on_flag == 1))
		{
		if((vibration_flag == 0)&&(wheeled_flag == 0))
			{
			Delay_1ms();
			if((key_rotate == 0)||(slave_nearby_actioned_flag == 0))
				{
				ElecMotor_ACW();

				key_rotated_on_flag = 0;
				slave_away_operation();		
				IDkey_speech_flash = 0;
				ID_speeched_flag = 0;
				}				
			}
		}
		
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
			
	if((sensor_detect == 0)||(horizontal_sensor == 0))
		{
		vibration_flag = 1;
		vibration_count = 0;			
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
//					if((sensor_detect == 0)&&(stolen_alarm_flag == 0)&&(transmiter_EN == 1))		
					if(((sensor_detect == 0)||(horizontal_sensor == 0))&&(stolen_alarm_flag == 0)&&(flashing_flag == 0)&&(transmiter_EN == 1))		
						{
						// judge LV is more than 2ms, if yes, it means a effective touch
						if(++sensor_1ststage_count >= 1)			
							{
							sensor_1ststage_count=0;
							
							// sensor trigge status progress to case 1.
							sensor_trigger_count = 1;
							// alarm speech for first touch
//							SCH_Add_Task(host_touch_speech, 0, 0);
//							host_touch_speech();
							Delay_500ms();
							Delay_500ms();
							Delay_500ms();
							Delay_500ms();
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
					if((sensor_detect == 0)||(horizontal_sensor == 0))
						{
						// LV for 2s, means a effective touch
						if(++sensor_2ndstage_count >= 1)
							{
							sensor_2ndstage_count = 0;
							sensor_trigger_count = 2;
							// alarm speech for 2nd touch
//							SCH_Add_Task(host_2ndtouch_speech, 0, 0);
							host_touch_speech();
							}
						}
					else
						{
						sensor_2ndstage_count = 0;
						}
					
					// if there is no touch in 4s, reset sensor trigger status, etc.
					if(++sensor_2ndstage_time >= 4000)
						{
						sensor_trigger_count = 0;
						sensor_2ndstage_count = 0;
						sensor_1ststage_count = 0;
						sensor_2ndstage_time = 0;
						}
					}
				break;
				
				// waiting for 3rd touch
				case 2:
					{
					if((sensor_detect == 0)||(horizontal_sensor == 0))
						{
						// 2s LV is a effective touch
						if(++sensor_3rdstage_count >= 1)
							{
							sensor_3rdstage_count = 0;
							// stolen alarm speech enable
							host_stolen_alarm1_EN = 1;
							host_stolen_alarm2_EN = 1;	
							sensor_3rdalarm_flag = 1;							
							}
						}
					else
						{
						sensor_3rdstage_count = 0;
						}
					
					// if there is no touch in 4s, reset all.
					if(++sensor_3rdstage_time >= 4000)
						{
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
				break;
				}
			
			// judge the wire broken, if yes, it means someone has cut the wire of magnet lock
			if((wire_broken == 0) && (wire_broken_count < 51))  
				{
				if(++wire_broken_count > 50)
					{
					host_stolen_alarm1_EN = 1;
					host_stolen_alarm2_EN = 1;	
					wire_broken_count = 51;
					wire_broken_flag = 1;
					}	
				}
			else if((wire_broken == 1)&&(sensor_3rdalarm_flag == 0))
				{
				wire_broken_count = 0;
				host_stolen_alarm1_EN = 0;
				host_stolen_alarm2_EN = 0;
				}
			
			if(ADC_check_result < 0x100)
				{
				battery_stolen_EN = 1;
				}
			else
				battery_stolen_EN = 0;
			}
//		}
	
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
				// flag fell, and reset raised
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
	
	// detect the horizontal sensor
	if(((horizontal_sensor == 0)||(sensor_detect == 0))&&(horizontal_vibration_count > 5000))
		{
		Delay(3);
		if((horizontal_sensor == 0)||(sensor_detect == 0))
			{
			horizontal_vibration = 1;
			horizontal_vibration_count = 0;
			}
		}
	if(++horizontal_vibration_count >= 5000)
		{
		horizontal_vibration_count = 5001;
		horizontal_vibration = 0;
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
			if((data_count == 0) && (received_data_buffer[0] == 0x01))
				{
				data_count = 1;
				}
			else if((data_count == 1) && (received_data_buffer[1] == 0x00))
				{
				data_count = 2;
				}
			else if((data_count == 2) && (received_data_buffer[2] == 0x1a))
				{
				data_count = 3;
				}
			else if((data_count == 3) && (received_data_buffer[3] == 0x9b))
				{
				data_count = 4;
				}
			else if((data_count == 4) && (received_data_buffer[4] == 0x15))
				{
				data_count = 5;
				}
			#ifdef ID
			else if((data_count == 5) && (received_data_buffer[5] == 0x95))
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