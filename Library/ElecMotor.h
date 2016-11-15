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
void ElecMotor_Delay_CW(void);
void ElecMotor_Delay_ACW(void);
void ElecMotor_test(void);
void ElecMotor_closecode(void);
void ElecMotor_stopcode(void);

#endif
											
/*---------------------------------------------------
	end of file
----------------------------------------------------*/