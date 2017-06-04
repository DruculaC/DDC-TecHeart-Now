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
<<<<<<< HEAD
=======
void ElecMotor_test(void);
>>>>>>> c6e65e4f0b4864e969ea86603842dc8b6ee1929b
void ElecMotor_closecode(void);
void ElecMotor_stopcode(void);

#endif
											
/*---------------------------------------------------
	end of file
----------------------------------------------------*/