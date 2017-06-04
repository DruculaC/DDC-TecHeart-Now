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

/*------- Public variable declarations --------------------------*/
extern tByte timer0_8H, timer0_8L, timer1_8H, timer1_8L;
extern bit enable_sensor_delayEN;

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

tByte lock_time = 0;

/*-----------------------------------------------------------*/
void main(void)
	{
	InitTimer(50, 100);
	
	// Set P2.4(PIN15) to Push Pull Output.
	P2M1 &= 0xef;
	P2M2 |= 0x10;
	
	Lock_EN = 1;
	SC_RST = 1;

	transmit_wire = 0;
	
	while(1)
		{
		sEOS_Go_To_Sleep();
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
	if(++timer0_count >= 10)		
		{
		// ��������0
		timer0_count = 0;
      transmiter_EN = ~transmiter_EN;
		}		
		
	if(Lock_EN == 0)
		{
		if(++lock_time >= 40)
			{
			lock_time = 0;
			Lock_EN = 1;
			}
		}
	}


/*------------------------------------------------------------------
	timerT1()
	��ʱ��1ÿ�������ִ�еĲ���
--------------------------------------------------------------------*/
	
void timerT1() interrupt interrupt_timer_1_overflow 			
	{
	// ��װ�ڶ�ʱ��1������
	TH1 = timer1_8H;				
	TL1 = timer1_8L;

	// �жϽ������Ƿ�Ϊ0�������0�����ʾ�������źŹ�����
	if(receive_wire == 1)					
		{
		// �����������0���򽫴˱�־λ��0����ʾ����Ϊ0����
		receive_wire_flag=0;
		
		// ���յ��͵�ƽ��ʱ�����������8ms�����¼���
		if(++receive_LV_count==150)		
			{
			receive_LV_count=0;
			}
		}
	else
		{
		if(receive_wire_flag==0)//˵����һ���͵�ƽ
			{
			receive_wire_flag=1;
//			one_receive_byte<<=1;

			if((receive_LV_count>20)&&(receive_LV_count<=80))//�͵�ƽ������ʱ��С��3ms����Ϊ0
				{
				one_receive_byte<<=1;
				one_receive_byte &= 0xfe;
				one_receive_byte_count++;
				receive_HV_count=0;
				}
			if(receive_LV_count > 80)//�͵�ƽ������ʱ�����4.5ms����Ϊ1
				{
				one_receive_byte<<=1;
				one_receive_byte |= 0x01;
				one_receive_byte_count++;
				receive_HV_count=0;
				}
			else
				{
				receive_HV_count++;	
				}

			receive_LV_count=0;
			}
		else
			{
			receive_HV_count++;
			if(receive_HV_count >= 100)
				{
				one_receive_byte_count=0;
				receive_wire_flag=1;
				data_count=0;
				Lock_EN = 1;			
				}		
			}
		}

	if(one_receive_byte_count == 8)//˵��һ���ֽڵ������Ѿ�������ȫ
		{
		one_receive_byte_count=0;
		received_data_buffer[data_count]=one_receive_byte;
		if(data_count==0&&received_data_buffer[0] == CmdHead)
			{
			data_count=1;
			}
		else if(data_count==1&&received_data_buffer[1] == MyAddress)
			{
			data_count=2;
			}
		else if(data_count==2)
			{
			receive_data_finished_flag = 1;
			data_count=0;
			}
		else 
			{
			data_count=0;
			}
		}

	if(receive_data_finished_flag==1)	//˵�����յ������ݣ���ʼ����
		{
		receive_data_finished_flag=0;	//����ձ�־
		switch(received_data_buffer[2])//����ָ��
			{
			case ComMode_1://���յ������������͹����ı���1���źţ�˵��������3M�ڣ���������
				{	
            Lock_EN = 0;
//				lock_time = 0;
				}
			break;
			}
		}
	}

/*---------------------------------------------------
	end of file
----------------------------------------------------*/