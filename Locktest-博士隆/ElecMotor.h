/*-----------------------------------------------------
	ElecMotor.h (v1.00)
	
	Header file for ElecMotor.c
-----------------------------------------------------*/

#ifndef _ElecMotor_H
#define _ElecMotor_H

#define CmdHead 0xc8
#define MyAddress 0xe0

#define ComMode_1 0xc1	// the verification/confirmation signal. once slave receive this signal, slave transmit a confirmation signal.

//-----public constants-------------------------------

//-----public function prototype----------------------
void ElecMotor_CW(void);
void ElecMotor_ACW(void);
void ElecMotor_code(void);	
void ElecMotor_Delay_CW(void);
void ElecMotor_Delay_ACW(void);

#endif
											
/*---------------------------------------------------
	end of file
----------------------------------------------------*/