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
bit ElecMotor_OC_flag = 0;
bit ElecMotor_OC_flag2 = 0;
tByte OC_count = 0;

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
	tByte i;
	
	lock_power = 1;	
		
	// ½«P0.2, PIN25ÉèÖÃÎª×¼Ë«ÏòÄ£Ê½
	P0M1 &= 0xfb;
	P0M2 &= 0xfb;
	wheeled_rotate = 0;
	
	// -----------------------------------------------
	ElecMotor_code();
//	Delay_100ms();
//	ElecMotor_code();

	MagentControl_1 = 1;
	MagentControl_2 = 0;
	ElecMotor_Delay_CW();
	MagentControl_1 = 1;
	MagentControl_2 = 1;
	
	for(i = 0; i < 7; i++)
		{
		#ifndef Locktest
		if(ElecMotor_OC_flag == 1)
			{
			ElecMotor_OC_flag = 0;
			// å…³é”2ç§’
			Delay_100ms();
			MagentControl_1 = 0;
			MagentControl_2 = 1;
			Delay_500ms();Delay_500ms();Delay_500ms();Delay_500ms();
			MagentControl_1 = 1;
			MagentControl_2 = 1;
			// å†æ¬¡å¼€é”
			Delay_100ms();Delay_100ms();
			ElecMotor_code();
			MagentControl_1 = 1;
			MagentControl_2 = 0;
			ElecMotor_Delay_CW();
			MagentControl_1 = 1;
			MagentControl_2 = 1;			
			}
		#endif
		
		#ifdef Locktest
		if(OC_count >= 1)
			Externalmotor = 0;
		#endif
		}
	
	
	wheeled_rotate = 1;
	// ½«P0.2, PIN25ÉèÖÃÎª×¼Ë«ÏòÄ£Ê½
	P0M1 |= 0x04;
	P0M2 &= 0xfb;
	
	ElecMotor_OC_flag = 0;

	Externalmotor = 1;
	
	lock_power = 0;
	}

/*-------------------------------------------------------
	ElecMotor_ACW()
	Electric Motor rotates anticlockwise.
--------------------------------------------------------*/
void ElecMotor_ACW(void)
	{
	tByte i;
	lock_power = 1;
		
	Externalmotor = 1;
	Lock_EN = 1;
//	Generator_lock = 0;
	
	ElecMotor_code();	
	MagentControl_1 = 0;
	MagentControl_2 = 0;
  Delay_100ms();

	MagentControl_1 = 0;
	MagentControl_2 = 1;
	ElecMotor_Delay_ACW();
	MagentControl_1 = 1;
	MagentControl_2 = 1;
	
	ElecMotor_OC_flag2 = 0;

	for(i = 0; i < 7; i++)
		{
		#ifndef Locktest
		if(ElecMotor_OC_flag == 1)
			{
			ElecMotor_OC_flag = 0;
			// å¼€é”2ç§’
			Delay_100ms();
			ElecMotor_code();
			MagentControl_1 = 1;
			MagentControl_2 = 0;
			Delay_500ms();Delay_500ms();Delay_500ms();Delay_500ms();
			MagentControl_1 = 1;
			MagentControl_2 = 1;
			// å†æ¬¡å…³é”
			Delay_100ms();
			MagentControl_1 = 0;
			MagentControl_2 = 1;
			ElecMotor_Delay_ACW();
			MagentControl_1 = 1;
			MagentControl_2 = 1;
			}
		#endif
		
		#ifdef Locktest
		if(OC_count >= 1)
			Externalmotor = 0;
		#endif
		}
	
	ElecMotor_OC_flag = 0;
		
		lock_power = 0;
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
	tWord zz;
	
	Delay_100ms();	Delay_100ms();Delay_100ms();	Delay_100ms();
	

	for(zz = 0; zz < 35; zz++)
		{
		if(ElecMotor_overcurrent == 1)
			ElecMotor_OC_flag = 1;
		
		if(ElecMotor_OC_flag == 0)
			Delay_10ms();
		}
	
	if(ElecMotor_OC_flag == 0)
		{
		Delay_500ms();
		#ifdef Old_lock	
		Delay_500ms();
		Delay_500ms();Delay_500ms();
		Delay_500ms();Delay_500ms();Delay_500ms();
		#endif
		}

/*	for(zz = 0; zz < 200; zz++)
		{
		if(ElecMotor_overcurrent == 1)
			ElecMotor_OC_flag = 1;
		
		if(ElecMotor_OC_flag == 0)
			Delay_10ms();
		}
*/	
//	Delay_500ms();
//	Delay_500ms();
//	Delay_500ms();
//	Delay_500ms();
//	Delay_500ms();


/*	#ifdef Z3
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	Delay_500ms();
	#endif
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
	tWord zz;

//	Delay_500ms();
//	Delay_500ms();
//	Delay_500ms();
	
	Delay_100ms();	Delay_100ms();Delay_100ms();Delay_100ms();
	
	
	for(zz = 0; zz < 35; zz++)
		{
		if(ElecMotor_overcurrent == 1)
			ElecMotor_OC_flag = 1;
		
		if(ElecMotor_OC_flag == 0)
			Delay_10ms();
		}
	
	if(ElecMotor_OC_flag == 0)
		{
		Delay_500ms();
		#ifdef Old_lock
		Delay_500ms();
		Delay_500ms();Delay_500ms();Delay_500ms();Delay_500ms();
		//Delay_500ms();Delay_500ms();Delay_500ms();
		#endif
		}

/*	for(zz = 0; zz < 300; zz++)
		{
		if(ElecMotor_overcurrent == 1)
			ElecMotor_OC_flag = 1;
		
		if(ElecMotor_OC_flag == 0)
			Delay_10ms();
		}
*/
//	#ifdef Guxingzha
//	Delay_500ms();
//	Delay_500ms();
//	#endif
	
	#ifdef Z3
	Delay_500ms();
	#endif

	if(ElecMotor_OC_flag == 1)
		OC_count += 1;

/*	if(ElecMotor_overcurrent == 0)
		{
		Delay_500ms();
		Delay_500ms();
		Delay_500ms();
		Delay_500ms();
		Delay_500ms();
		Delay_500ms();
		}
*/
	}

/*---------------------------------------------------
	end of file
----------------------------------------------------*/
