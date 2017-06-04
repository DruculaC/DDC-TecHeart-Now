/*---------------------------------------------------
	ElecMotor.c (v1.00)
	
	All programs related to Electric Motor in Main board
---------------------------------------------------*/	

#include "main.h"
#include "port.h"

#include "ElecMotor.h"
#include "Delay.h"

/*------- Public variable declarations -----------------*/
tByte myTxRxData[7];

bit CW_flag = 0;
tByte Elecmotor_duration_G;
bit ElecMotor_OC_flag = 0;
bit ElecMotor_OC_flag2 = 0;
tByte OC_count = 0;

/*-------------------------------------------------------
	ElecMotor_CW()
	Electric Motor rotates clockwise.
--------------------------------------------------------*/
void ElecMotor_CW(void)
	{
	tByte i, itime;
	
	// 发送两次密码
	ElecMotor_code();
	Delay_100ms();
	ElecMotor_code();

	MagentControl_1 = 1;
	MagentControl_2 = 0;
	ElecMotor_Delay_CW();
	MagentControl_1 = 1;
	MagentControl_2 = 1;
	
	for(i = 0; i < 7; i++)
		{
		if(ElecMotor_OC_flag == 1)
			{
			ElecMotor_OC_flag = 0;
			// 关锁2秒
			Delay_ms(LOOP_TIMEOUT_INIT_100ms);
			MagentControl_1 = 0;
			MagentControl_2 = 1;
			
			for(itime = 0; itime < 4; itime++)
				Delay_ms(LOOP_TIMEOUT_INIT_500ms);
			
			MagentControl_1 = 1;
			MagentControl_2 = 1;
			// 再次开锁
			
			for(itime = 0; itime < 2; itime++)
				Delay_ms(LOOP_TIMEOUT_INIT_100ms);
			
			ElecMotor_code();
			MagentControl_1 = 1;
			MagentControl_2 = 0;
			ElecMotor_Delay_CW();
			MagentControl_1 = 1;
			MagentControl_2 = 1;			
			}
		// 如果大于5次，则亮红灯。
		if(OC_count >= 5)
			Generator_lock = 0;
		}
		
	OC_count = 0;
	ElecMotor_OC_flag = 0;
	}

/*-------------------------------------------------------
	ElecMotor_ACW()
	Electric Motor rotates anticlockwise.
--------------------------------------------------------*/
void ElecMotor_ACW(void)
	{
	tByte i, itime;

//	Generator_lock = 0;
	
	MagentControl_1 = 0;
	MagentControl_2 = 1;
	ElecMotor_Delay_ACW();
	MagentControl_1 = 1;
	MagentControl_2 = 1;
	
	ElecMotor_OC_flag2 = 0;

	for(i = 0; i < 7; i++)
		{
		if(ElecMotor_OC_flag == 1)
			{
			ElecMotor_OC_flag = 0;
			// 开锁2秒
			Delay_ms(LOOP_TIMEOUT_INIT_100ms);
			ElecMotor_code();
			MagentControl_1 = 1;
			MagentControl_2 = 0;
			for(itime = 0; itime < 4; itime++)
				Delay_ms(LOOP_TIMEOUT_INIT_500ms);
			MagentControl_1 = 1;
			MagentControl_2 = 1;
			// 再次关锁
			Delay_ms(LOOP_TIMEOUT_INIT_100ms);
			MagentControl_1 = 0;
			MagentControl_2 = 1;
			ElecMotor_Delay_ACW();
			MagentControl_1 = 1;
			MagentControl_2 = 1;
			}
		

		if(OC_count >= 5)
			Generator_lock = 0;
		}
	
	OC_count = 0;
	ElecMotor_OC_flag = 0;
	}

/*--------------------------------------------------
	ElecMotor_code()
	send the code to Electric Motor.
---------------------------------------------------*/
void ElecMotor_code(void)	
	{	
	tByte i,n;
	myTxRxData[0]=CmdHead;
	myTxRxData[1]=MyAddress;
	myTxRxData[2]=ComMode_1;
	
	for(i=0;i<3;i++)
		{
		for(n=0;n<8;n++)
			{
			if((myTxRxData[i]&0x80) == 0x80)
				{
				MagentControl_2 = 0;
				Delay_12ms();
				}
			else
				{
				MagentControl_2 = 0;
				Delay_5ms();
				}
			MagentControl_2 = 1;	
			myTxRxData[i] <<= 1;
			Delay_5ms();
			}
		}
	}

/*----------------------------------------------------
	ElecMotor_Delay_CW()
	Delay program for Electric Motor.
-----------------------------------------------------*/
void ElecMotor_Delay_CW(void)
	{
	tWord zz, itime;
	
	Delay_100ms();	Delay_100ms();Delay_100ms();	Delay_100ms();
	
	// 400ms后的检测电流，每10ms检测一次。
	for(zz = 0; zz < 20; zz++)
		{
		if(ElecMotor_overcurrent == 1)
			ElecMotor_OC_flag = 1;
		
		if(ElecMotor_OC_flag == 0)
			Delay_10ms();
		}
	
	if(ElecMotor_OC_flag == 0)
		{
		for(itime = 0; itime < 6; itime++)
			Delay_ms(LOOP_TIMEOUT_INIT_500ms);
		}

/*	// 对于全螺纹，则是后半段检测电流，每10ms检测一次。
	for(zz = 0; zz < 200; zz++)
		{
		if(ElecMotor_overcurrent == 1)
			ElecMotor_OC_flag = 1;
		
		if(ElecMotor_OC_flag == 0)
			Delay_10ms();
		}
*/		
	if(ElecMotor_OC_flag == 1)
		OC_count += 1;
	}
/*----------------------------------------------------
	ElecMotor_Delay_ACW()
	Delay program for Electric Motor.
-----------------------------------------------------*/
void ElecMotor_Delay_ACW(void)
	{
	tWord zz, itime;

	
	Delay_100ms();	Delay_100ms();Delay_100ms();Delay_100ms();
	
	// 400ms后的检测电流，每10ms检测一次。
	for(zz = 0; zz < 20; zz++)
		{
		if(ElecMotor_overcurrent == 1)
			ElecMotor_OC_flag = 1;
		
		if(ElecMotor_OC_flag == 0)
			Delay_10ms();
		}

	if(ElecMotor_OC_flag == 0)
		{
		for(itime = 0; itime < 6; itime++)
			Delay_ms(LOOP_TIMEOUT_INIT_500ms);
		}

/*	for(zz = 0; zz < 300; zz++)
		{
		if(ElecMotor_overcurrent == 1)
			ElecMotor_OC_flag = 1;
		
		if(ElecMotor_OC_flag == 0)
			Delay_10ms();
		}

*/
	if(ElecMotor_OC_flag == 1)
		OC_count += 1;
	}

/*---------------------------------------------------
	end of file
----------------------------------------------------*/
