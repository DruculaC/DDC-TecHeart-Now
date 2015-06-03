/*-----------------------------------------------------
	ElecMotor.h (v1.00)
	
	Header file for ElecMotor.c
-----------------------------------------------------*/

#ifndef _ElecMotor_H
#define _ElecMotor_H

//-----public constants-------------------------------

//-----public function prototype----------------------
void InitElecmotor(void);
void ElecMotor_CW(void);
void ElecMotor_ACW(void);
void ElecMotor_code(void);	
void ElecMotor_Delay(void);
void ElecMotor_Delay_ACW(void);

#endif
											
/*---------------------------------------------------
	end of file
----------------------------------------------------*/