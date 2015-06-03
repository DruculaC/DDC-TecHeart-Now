/*---------------------------------------------------
	main.c (v1.00)
	
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
tByte nearby_away_interval = 0;		//�����뿪�Ϳ���ʱ��������ʾ�Ϳ�������ʱ����

tByte raised_alarm_count = 0;    //������̧����򸽻����������źŵĴ���
tByte fell_alarm_count=0;        //�������غ��򸽻����������źŵĴ���
tWord timer0_count=0;		// counter for timer0, increment every ticket 			
tByte received_data_buffer[7]={0x00,0x00,0x00,0x00,0x00,0x00,0x00};		//�������ݻ���
bit receive_data_finished_flag = 0;		//������һ��������ɺ󣬴˱�־λ��1
tByte data_count = 0;				//�������ݻ����λ�����������ڼ�����������
tByte one_receive_byte = 0;		//�������ݵ�һ���ֽڣ���������丳ֵ��received_data_buffer���Ӧ���ֽ�
tByte one_receive_byte_count = 0;			//one_receive_byte��8λ���˼����������յ��ڼ�λ��ÿ�μ�����8��ʱ�����һ���ֽڽ�����ɡ�
bit receive_wire_flag = 1;		//����ͨ���ߵı�־λ��1�����ߵ�ƽ��0�����͵�ƽ��ÿ��timer1���ʱ���ж�P1.1һ�Ρ��Դ��������Ƿ�Ϊһ����ȷ���½���
tByte receive_HV_count = 0;		//��ʱ��T1��û���źŵ�����ʱ�򣬶Ըߵ�ƽ������һ������ĳ��ֵ����one_receive_byte_count��0
tByte receive_LV_count = 0;		//ÿ��timer1���ʱ�жϽ��������ΪLV���������1���Դ��������͵�ƽ��ʱ��
tByte fell_wire_time=0;          //���ؼ���ߣ����͵�ƽ��ʱ��
tByte raise_wire_time=0;			//̧�����ߣ����͵�ƽ��ʱ��
tWord raised_fell_number = 0;				//���ػ���̧������󣬼���������һ����ֵ�󣬽������־λһ�����㡣
bit raised_fell_once_flag = 0;			//raised_fell_flag�Ƿ�������־���������־������1.Ȼ���������ָ����ػ��߻ָ�̧��ʱ���˱�־λ��λ��
tByte key_rotated_on_flag=0;			//�綯�������رձ�־λ��1��ʾ�綯�������ˣ�0��ʾ�綯���ر���
tWord ADC_check_result = 0;		//��ΪAD���ֵ
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
tWord match_button_count = 0;

// ------ Private variable definitions -----------------------------
tWord stolen_alarm_count = 0;
bit raised_alarm_flag = 0;
bit fell_alarm_flag = 0;
bit Host_battery_high_flag = 0;
bit wire_broken_EN = 0;
tWord Mode_detect_count = 0;
bit Mode_select = 0;	// ģʽѡ��0��ʾ�Զ�ģʽ��1��ʾ�ֶ�ģʽ

bit powersave_enable = 0;

bit itrpt_EN = 1;
tByte itrpt_count = 0;
bit idle_EN = 0;

bit Auto_transmit = 0;			// 0��ʾ���Զ��������ݣ�1��ʾ�Զ��������ݽ�����֤
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

bit match_button_touched = 0;

bit battery_LV_flag = 0;
tByte battery_LV_flag_count = 0;
bit battery_HV_flag = 0;
tByte battery_HV_flag_count = 0;
tByte battery_HV_speech_count = 0;
bit battery_HV_speech_over = 0;

code tByte IDkey6 _at_ 0x003000;
code tByte IDkey7 _at_ 0x003001;
code tByte IDkey8 _at_ 0x003002;
code tByte IDkey9 _at_ 0x003003;
code tByte IDkey10 _at_ 0x003004;
code tByte IDkey11 _at_ 0x003005;

/*--------------------------------------------------------------*/

void main(void)
	{
	InitVoice();
	InitUART600();
   TR0 = 1;

	// �����жϳ�ʼ��
	press_open_button = 1;
	press_close_button = 1;
	
	KBLS1 |= 0x03;
	KBLS0 |= 0x03;
	KBIF &= 0xfe;
	KBIE |= 0x02;
	EA = 1;

	Moto_EN = 1;		//��ʼ�����ر����
	transmit_wire = 1;
	voice_EN = 0;		  	//����ʱ�������Źر�
	P10=1;
	
	
	stolen_alarm_count = 0;			//�屨��������
	stolen_alarm_flag = 0;			//�屨����־

	transmiter_EN = 0;		// turn off the transmitter
	receiver_EN = 0;		// turn on the receiver

	transceiver_power_enable = 1;         // �ϵ�ʱ����ģ���Դ�ر�
	
	while(1)
		{				
      hSCH_Dispatch_Tasks();

		if(idle_EN == 1)
			{
			EKB = 1;
			idle_EN = 0;
			PCON |= 0x02;			
			}
		
			// ������������
		if(stolen_alarm_flag == 1)		
			{
			// ������ʾ�������
			Alarm_stolen_speech();		
			Moto_Vibration();          
			stolen_alarm_flag = 0;
			}	

		// ̧���źű�����ÿ�α������0������ٴνӵ����������һ����˵������ÿ��̧��ֻ��4�顣
		if(raised_alarm_flag == 1)	
			{
			Alarm_raised_speech();		
			Moto_Vibration();         
			raised_alarm_flag = 0;
			}

		// �����źű�����ÿ�α������0������ٴνӵ����������һ����˵������ÿ�ε���ֻ��4�顣
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
         Host_battery_high_alarm_speech();
			battery_stolen_EN = 0;
			Moto_Vibration();         			
			}
		
//		sEOS_Go_To_Sleep();			
		}  
	}


/*------------------------------------------------------------------
	timerT0()
	��ʱ��0ÿ�������ִ�еĲ���
--------------------------------------------------------------------*/

void timer0() interrupt interrupt_timer_0_overflow
	{
	// ��װ�ڶ�ʱ����ʱ������
	TH0 = timer0_8H;
	TL0 = timer0_8L;

	// ����һ��ÿ3s�Ĳ���
	if(++timer0_count >= 2000)		
		{
			
		// ÿ��3s��һ�ε�����⣬��������صĵ�����ʾ
		CheckADC();
		
		if(ADC_check_result <= 0x368)                 // 3.11V/3.64V ��������
			{
			battery_LV_flag = 1;
			battery_HV_flag = 0;
			battery_HV_speech_over = 0;
			}
		else if(ADC_check_result >= 0x37a)            // 3.52V/4.03V ��������
			{
			battery_HV_flag = 1;
			battery_LV_flag = 0;
			}
		else
			{
			battery_LV_flag = 0;
			battery_HV_flag = 0;
			}
				
/*		if((battery_HV_flag == 1)&&(battery_HV_speech_over == 0))
			{
			if(++battery_HV_flag_count > 3)
				{
				Battery_high_alarm_speech();
				battery_HV_flag_count = 0;
				if(++battery_HV_speech_count >= 3)
					{
					battery_HV_speech_over = 1;
					battery_HV_speech_count = 0;
					}
				}
			}
*/		
		if(battery_LV_flag == 1)
			{
			if(++battery_LV_flag_count > 20)
				{
				Battery_low_alarm_speech();
				battery_LV_flag_count = 0;
				}			
			}
						
		if(match_button_flag6 == 1)
			{
			if(++match_button_flag6count > 10)
				{
				match_button_HVcount = 0;
				match_button_LVcount = 0;
				match_button_flag1 = 0;
				match_button_flag2 = 0;
				match_button_flag3 = 0;
				match_button_flag4 = 0;
				match_button_flag5 = 0;
				match_button_flag6 = 0;
				match_moto_EN = 0;
				match_button_flag6count = 0;				
				}			
			}		
		// ��������0
		timer0_count = 0;
		}
		
	if((toggle_button == 1)&&(idle_EN == 0))
		{
		transceiver_power_enable = 1;
		receiver_EN = 0;
		transmiter_EN = 0;
		RXD = 0;
		TXD = 0;
		idle_EN = 1;
		}
	
	if((transceiver_power_enable == 0)&&(match_button_flag6 == 0))
		{
		if(++receiver_EN_count > 20)
			{
			transceiver_power_enable = 1;
			receiver_EN = 0;
			RXD = 0;
			TXD = 0;
//			receiver_EN = 1;
			receiver_EN_count = 0;
			}
		}	
	
	if((transceiver_power_enable == 1)&&(match_button_flag6 == 0))
		{		
		if(++receiver_DisEN_count > 1500)
			{
			transceiver_power_enable = 0;
			UART_Send_Data_F(ComMode_1);
			receiver_EN = 0;
			RXD = 1;
//			receiver_EN = 0;
			receiver_DisEN_count = 0;
			}

		}
	
	if(match_button == 0)
		{
		match_button_HVcount = 0;
					
		if(++match_button_LVcount > 4000)
			{
			match_button_LVcount = 4002;
			match_button_flag1 = 0;
			match_button_flag2 = 0;
			match_button_flag3 = 0;
			match_button_flag4 = 0;
			match_button_flag5 = 0;
			}
		else
			{
			match_button_flag1 = 1;
			if(match_button_flag2 == 1)
				match_button_flag3 = 1;
			if(match_button_flag4 == 1)
				match_button_flag5 = 1;
			}
		
		battery_LV_flag = 0;
		battery_HV_flag = 0;
		}
	else
		{
		match_button_LVcount = 0;
		
		if(match_button_flag1 == 1)
			match_button_flag2 = 1;
			
		if(match_button_flag3 == 1)
			match_button_flag4 = 1;
		
		if(match_button_flag5 == 1)
			{
			match_button_flag6 = 1;
			transceiver_power_enable = 0;
			receiver_EN_count = 0;
			receiver_EN = 0;
			RXD = 1;
			if(match_moto_EN == 0)
				{
				Moto_Vibration(); 
				match_moto_EN = 1;
				}
			}
			
		if(++match_button_HVcount > 4000)
			{
			match_button_HVcount = 4002;
			match_button_flag1 = 0;
			match_button_flag2 = 0;
			match_button_flag3 = 0;
			match_button_flag4 = 0;
			match_button_flag5 = 0;
			match_moto_EN = 0;
//			match_button_flag6 = 0;
			}
		}
		
	if(IDflash_EN == 1)
		{
		IDflash_EN = 0;
		match_button_flag1 = 0;
		match_button_flag2 = 0;
		match_button_flag3 = 0;
		match_button_flag4 = 0;
		match_button_flag5 = 0;
		match_button_flag6 = 0;
		match_moto_EN = 0;
//		SCH_Add_Task(Self_learn_programming_F, 0, 0);
      Self_learn_programming_F();
		Moto_Vibration();
		Delay(10);
		Moto_Vibration();
		match_button_flag6count = 0;
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
		
		if(match_button_flag6 == 0)
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
				IDflash_EN = 1;
				}
			else 
				{
				data_count = 0;
				}
			}		
		}
	}

/*------------------------------------------------------------------
	timerT1()
	��ʱ��1ÿ�������ִ�еĲ���
	
void timerT1() interrupt interrupt_timer_1_overflow 			
	{
	// ��װ�ڶ�ʱ��1������
	TH1 = timer1_8H;				
	TL1 = timer1_8L;

	// receive a tyte
	receive_byte();
	
	// receive a word after every byte
	receive_word();

	if(receive_data_finished_flag==1)	//˵�����յ������ݣ���ʼ����
		{
		receive_data_finished_flag=0;	//����ձ�־
		switch(received_data_buffer[2])//����ָ��
			{
			case ComMode_2:
				{
				battery_stolen_EN = 1;
				Moto_Vibration();          
				}
		   break;
			
			case ComMode_3:
				{
				stolen_alarm_flag = 1;
				Moto_Vibration();         

				raised_alarm_count=0;
				raised_alarm_flag=0;
				fell_alarm_count=0;
				fell_alarm_flag=0;
				}
			break;
		
			case ComMode_4:
				{
				raised_alarm_flag=1;
				Moto_Vibration();         

				stolen_alarm_count=0;
				stolen_alarm_flag=0;
				fell_alarm_count=0;
				fell_alarm_flag=0;
				}
			break;

			case ComMode_5:
				{
				fell_alarm_flag=1;	
				Moto_Vibration();         

				stolen_alarm_count=0;
				stolen_alarm_flag=0;
				raised_alarm_count=0;
				raised_alarm_flag=0;
				}
			break;

			case ComMode_6:
				{
				wire_broken_EN = 1;
				Moto_Vibration();         
				}
			break;
			}
		}
	}
--------------------------------------------------------------------*/

/*-----------------------------------------------------------
	KBI_ISR()
	�����жϣ�ʹоƬ��ʡ��ģʽ�л���
-----------------------------------------------------------*/
void KBI_ISR(void) interrupt 7
	{
	EKB = 0;
	KBIF &= 0xfc;
	transceiver_power_enable = 0;
	receiver_EN = 0;
	RXD = 1;
	match_button_touched = 1;
//	EKB = 1;
	}
		
/*---------------------------------------------------
	end of file
----------------------------------------------------*/