/*---------------------------------------------------
	main.c (v1.00)
	
	DDC-Z program, for electrocar.
----------------------------------------------------*/

//电动车
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
tWord sensor_3rdstage_interval = 0;		//传感器在第三阶段中，每次有效低电平计数之间的时间间隔。在这期间的低电平不认为有效。
bit raised_fell_flag = 0;					//倒地或者抬起触发后，此标志位置1
tByte sensor_trigger_count=0;		//传感器触发计数
tWord sensor_2ndstage_time=0;		//传感器进入第二阶段后流逝时间的计数
tByte sensor_1ststage_count=0;	//传感器第一阶段判断低电平的计数
tByte raise_wire_time=0;			//抬起检测线，检测低电平的时间
tWord raised_fell_number = 0;				//倒地或者抬起出发后，计数，到达一定数值后，将其与标志位一起清零。
bit raised_fell_once_flag = 0;			//raised_fell_flag是否曾经标志过，如果标志过则置1.然后主机被恢复倒地或者恢复抬起时，此标志位复位。
tByte key_rotated_on_flag = 0;			//电动车开启关闭标志位，1表示电动车开启了，0表示电动车关闭了
tWord ADC_check_result = 0;		//作为AD检测值
tWord load_battery_result = 0;
tByte wire_broken_count = 0;		// 作为断线后的时间检测
bit battery_stolen_EN = 0;			// 作为电池被盗的使能端
tByte battery_stolen_count = 0;	// 作为电池被盗的报警次数
bit horizontal_vibration = 0;		// 表示垂直传感器在振动，此时就算关钥匙，也不能执行关钥匙的操作�
tWord horizontal_vibration_count = 0;	//垂直传感器触发后，对时间进行计数。
bit vibration_flag = 0;
tWord vibration_count = 0;
bit wire_broken_flag = 0;			// 剪断锁线的标志位
bit IDkey_flag = 0;			// 当ID卡靠近时认证通过后置1，
tByte IDkey_count = 0;		// ID卡认证通过后，计时1分钟，使钥匙能转动。
tByte enable_sensor_delay_count = 0;		// 传感器延迟的时间
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
	
	// 閰嶇疆P0.1涓鸿緭鍏ユā寮�
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