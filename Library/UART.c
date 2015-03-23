/*-----------------------------------------------------------------*

	Copyright(c) 2014 Techeart Technology Corp. All rights reserved.
    Date: 11/Nov/2014
    E-Mail: DruculaC@gmail.com

*-----------------------------------------------------------------*/

#include "main.h"
#include "port.h"

#include "uart.h"
#include "Timer.h"

/*--------------------------------------------------------------
	Initialize UART port.
--------------------------------------------------------------*/
void InitUART9600()
	{
	AUXR1 &= 0xbf;			// Choose P1.0 P1.1 as UART port. 
	SCON = 0x5a;        	// Choose Mode 2.
	TMOD = 0x20;        	// Timer 1 is Mode 2, auto-reload.
	TH1 = TL1 = -(OSC_FREQ/12/32/BAUD9600);
	TR1 = 1;					// Start Timer 1.

	InitT0(1);
	TI = 0;
	RI = 0;
	ES = 1;
	ET0 = 1;
	PS = 1;
	EA = 1;
	}

void InitUART600()
	{
	AUXR1 &= 0xbf;			// Choose P1.0 P1.1 as UART port. 
	SCON = 0x5a;        	// Choose Mode 2.
	TMOD = 0x20;        	// Timer 1 is Mode 2, auto-reload.
	TH1 = TL1 = -(OSC_FREQ/12/32/BAUD600);
	TR1 = 1;					// Start Timer 1.

	InitT0(1);
	TI = 0;
	RI = 0;
	ES = 1;
	ET0 = 1;
	PS = 1;
	EA = 1;
	}

/*-----------------------------------------------------------
	Send a Byte.
-----------------------------------------------------------*/

void SendData(tByte dat)
	{
	SBUF = dat;
	while(!TI);
	TI = 0;	
	}

/*-----------------------------------------------------------
	Send multiple bytes. 
-----------------------------------------------------------*/
void SendNByte(tByte *buf, tByte len)
	{
	while(len--)
		{
		SendData(*buf++);
		}
	}

/*---------------------------------------------------
	end of file
----------------------------------------------------*/