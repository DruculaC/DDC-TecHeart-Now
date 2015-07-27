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

/*------- Public variable definitions --------------------------*/
bit ID_certificated_flag = 0;			// Flagged once ID card or Slave certificated successfully.
tByte ID_certificated_numbers = 0;	// Numbers of ID certificated.

bit Host_stolen_alarming = 0;			// Host stolen is alarming, don't detect vibration for 1st stage.

// ------ Private variable definitions -----------------------------
bit position_sensor_EN=0;  		//λ�ô�������������̧�𴫸����ܿ��أ�1��ʱ�򣬼��������������
bit EN_host_stolen_alarming = 0;      //�ж�Ϊ����������ĵ�һ������ʹ��
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
tByte Open_action_flag = 0;			//�綯�������رձ�־λ��1��ʾ�綯�������ˣ�0��ʾ�綯���ر���
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
tByte After_IDcert_timecount = 0;		// ID����֤ͨ���󣬼�ʱ1���ӣ�ʹԿ����ת����
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

/*------- Private variable declaratuions --------------------------*/

void main()
	{
	InitT0(1);
	TR0 = 1;
	ET0 = 1;
	PS = 1;
	
	InitSensor();

	Self_learn_speech();
	
	sensor_EN = 1;
	EA = 1;
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
	TH0 = timer0_8H;
	TL0 = timer0_8L;
	
//	if((sensor_detect == 0)||(horizontal_sensor == 0))
//		{
//	   ID_speech();
//		}
	
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
					if(((sensor_detect == 0)||(horizontal_sensor == 0))&&(Host_stolen_alarming == 0)&&(flashing_flag == 0)&&(transmiter_EN == 1))		
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
                     ID_speech();
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
							sensor_trigger_count = 0;
							// alarm speech for 2nd touch
//							SCH_Add_Task(host_2ndtouch_speech, 0, 0);
							Self_learn_speech();
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
							EN_host_stolen_alarming = 1;
							host_stolen_alarm2_EN = 1;	
							sensor_3rdalarm_flag = 1;	
							Stolen_alarm_reset_count = 0;
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
						sensor_3rdstage_count = 0;
						}
					}
				break;
				}
			
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
					Silence_Flag = 1;
					}
				if(++ID_certificated_numbers >= 11)
					{
					never_alarm = 1;
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
//						never_alarm = ~never_alarm;
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