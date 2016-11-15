/*---------------------------------------------------
	ElecMotor.c (v1.00)
	
	All programs related to Electric Motor in Main board
---------------------------------------------------*/	

#include "main.h"
#include "port.h"

#include "ElecMotor.h"
#include "Delay.h"
#include "communication.h"

/*------- Public variable declarations -----------------*/
extern tByte myTxRxData[7];

bit CW_flag = 0;
tByte Elecmotor_duration_G;

/*-------------------------------------------------------
	InitElecmotor()
--------------------------------------------------------*/
void InitElecmotor(void)
	{
	#ifdef Guxingzha
	ElecMotor_ACW();
	#endif
	
	#ifdef Suidongzha
//	ElecMotor_CW();
	#endif
	}
	
/*-------------------------------------------------------
	ElecMotor_CW()
	Electric Motor rotates clockwise.
--------------------------------------------------------*/
void ElecMotor_CW(void)
	{
	// 将P0.2, PIN25设置为准双向模式
	P0M1 &= 0xfb;
	P0M2 &= 0xfb;
	wheeled_rotate = 0;
	
	ElecMotor_code();

	MagentControl_1 = 1;
	MagentControl_2 = 0;
	ElecMotor_Delay_CW();
	MagentControl_1 = 1;
	MagentControl_2 = 1;
	
	wheeled_rotate = 1;
	// 将P0.2, PIN25设置为准双向模式
	P0M1 |= 0x04;
	P0M2 &= 0xfb;

//	Externalmotor = 0;
	}

/*-------------------------------------------------------
	ElecMotor_ACW()
	Electric Motor rotates anticlockwise.
--------------------------------------------------------*/
void ElecMotor_ACW(void)
	{
	Externalmotor = 1;
	Lock_EN = 1;
//	Generator_lock = 0;
	
	MagentControl_1 = 0;
	MagentControl_2 = 1;
	ElecMotor_Delay_ACW();
	MagentControl_1 = 1;
	MagentControl_2 = 1;
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

/*--------------------------------------------------
	ElecMotor_closecode()
	send the code to Electric Motor.
---------------------------------------------------*/
void ElecMotor_closecode(void)	
	{	
	tByte i,n;
	myTxRxData[0]=CmdHead;
	myTxRxData[1]=MyAddress;
	myTxRxData[2]=ComMode_2;
	
	for(i=0;i<3;i++)
		{
		for(n=0;n<8;n++)
			{
			if((myTxRxData[i]&0x80) == 0x80)
				{
				TXD = 0;
				Delay_12ms();
				}
			else
				{
				TXD = 0;
				Delay_5ms();
				}
			TXD = 1;	
			myTxRxData[i] <<= 1;
			Delay_5ms();
			}
		}
	}
/*--------------------------------------------------
	ElecMotor_stopcode()
	send the code to Electric Motor.
---------------------------------------------------*/
void ElecMotor_stopcode(void)	
	{	
	tByte i,n;
	myTxRxData[0]=CmdHead;
	myTxRxData[1]=MyAddress;
	myTxRxData[2]=ComMode_3;
	
	for(i=0;i<3;i++)
		{
		for(n=0;n<8;n++)
			{
			if((myTxRxData[i]&0x80) == 0x80)
				{
				TXD = 0;
				Delay_12ms();
				}
			else
				{
				TXD = 0;
				Delay_5ms();
				}
			TXD = 1;	
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
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();


	#ifdef Z3
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	#endif
	
	if(ElecMotor_overcurrent == 0)
		{
		Delay_500ms();
		Delay_500ms();
		}
	}
/*----------------------------------------------------
	ElecMotor_Delay_ACW()
	Delay program for Electric Motor.
-----------------------------------------------------*/
void ElecMotor_Delay_ACW(void)
	{

	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	
	#ifdef Guxingzha
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	#endif
	
	#ifdef Z3
	Delay_500ms();
	#endif

	if(ElecMotor_overcurrent == 0)
		{
		Delay_500ms();
		Delay_500ms();
		}
	}

/*----------------------------------------------------
	ElecMotor_test()
	Delay program for Electric Motor.
-----------------------------------------------------*/
void ElecMotor_test(void)
	{
	if(CW_flag == 1)
		{
		Elecmotor_duration_G += 1;
		if(Elecmotor_duration_G >= 10)
			{
			Elecmotor_duration_G = 0;
			ElecMotor_CW();
			CW_flag = 0;
			}
		}
	else
		{
		Elecmotor_duration_G += 1;
		if(Elecmotor_duration_G >= 10)
			{
			Elecmotor_duration_G = 0;
			ElecMotor_ACW();
			CW_flag = 1;
			}		
		}
	}

/*---------------------------------------------------
	end of file
----------------------------------------------------*/
