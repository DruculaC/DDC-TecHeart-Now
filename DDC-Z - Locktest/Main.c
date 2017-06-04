/*---------------------------------------------------
	main.c (v1.00)
	
	DDC-Z program, for electrocar.
----------------------------------------------------*/

//�綯��
#include "Main.h"
#include "port.h"

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
#include "ElecMotor.h"
#include "hSch51.h"

                                        
/*------- Public variable declarations --------------------------*/
extern tByte timer0_8H, timer0_8L, timer1_8H, timer1_8L;
extern bit enable_sensor_delayEN;
extern bit sensor_EN;


// ------ Private variable definitions -----------------------------
tWord sensor_3rdstage_interval = 0;		//�������ڵ����׶��У�ÿ����Ч�͵�ƽ����֮���ʱ�����������ڼ�ĵ͵�ƽ����Ϊ��Ч��
bit raised_fell_flag = 0;					//���ػ���̧�𴥷��󣬴˱�־λ��1
tByte sensor_trigger_count=0;		//��������������
tWord sensor_2ndstage_time=0;		//����������ڶ��׶κ�����ʱ��ļ���
tByte sensor_1ststage_count=0;	//��������һ�׶��жϵ͵�ƽ�ļ���
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
bit wire_broken_G = 0;
extern tByte OC_count;
tByte Locktest_count = 0;
extern tWord timer0_count;			

#define Maxcount 50

/*------- Private variable declarations --------------------------*/

void main()
	{	
	InitVoice();
	ID_speech();
	
	// lock the external motor,
	// InitElecmotor();	

	InitT0(50);
	
	// 配置P0.1为输入模式
	P0M1 |= 0x02;
	P0M2 &= 0xfd;

	
//	hSCH_Add_Task(ElecMotor_CW, 20, 100, 1);
	Externalmotor = 1;
	
	TR0 = 1;
	ET0 = 1;
	PS = 1;
	EA = 1;

//	hSCH_Start();

	while(1)
		{		
//		hSCH_Dispatch_Tasks();
		}
	}

void timer0() interrupt interrupt_timer_0_overflow
	{
	// manually reload timer0 configuration
	TH0 = timer0_8H;
	TL0 = timer0_8L;
	transmiter_EN = ~transmiter_EN;

	if((Externalmotor == 1)&&(wire_broken_G == 0))
		Locktest_count += 1;

	if((Externalmotor == 1)&&(Locktest_count <= Maxcount)&&(wire_broken_G == 0))
		{			
		Delay_500ms();Delay_500ms();Delay_500ms();Delay_500ms();Delay_500ms();
		ElecMotor_ACW();
		Delay_500ms();Delay_500ms();Delay_500ms();Delay_500ms();Delay_500ms();
		ElecMotor_CW();
		}
		
	if(wire_broken == 0)
		{
		Delay_50ms();
		if(wire_broken == 0)
			{
			wire_broken_G = 1;
			Externalmotor = 1;
			Locktest_count = 0;
			OC_count = 0;
			}
		}
	else		
		{		
		if(wire_broken_G == 1)
			{
			wire_broken_G = 0;
			}
		}
	
	if(++timer0_count >= 40)
		{
		// reset timer0 ticket counter every 2s
		timer0_count=0;
	
		if(wire_broken == 1)
			{
			if((Locktest_count <= Maxcount)&&(Externalmotor == 0))
				{
				Self_learn_speech();		
				}
			
			if(Locktest_count > Maxcount)
				{				
				Locktest_count = Maxcount+1;
				ID_speech();
				Externalmotor = ~Externalmotor;
				}				
			}
		}
	}
/*---------------------------------------------------
	end of file
----------------------------------------------------*/