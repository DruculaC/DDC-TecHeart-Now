/*-----------------------------------------------------
	operation.h (v1.00)
	
	header file for operation.c
-----------------------------------------------------*/

#ifndef _OPERATION_H
#define _OPERATION_H

//-----public constants-------------------------------

//-----public function prototype----------------------
void slave_away_operation(void);
void slave_nearby_operation(void);
void InitSensor(void);
void Host_stolen_action(void);
void ENsensor_afterIDcert(void);
void ENsensor_After_CloseLock(void);
void SelfLearn_Reset(void);
void Fell_Alarm_to_Slave(void);
void Raise_Alarm_to_Slave(void);
void Batstolen_Alarm_to_Slave(void);

#endif
											
/*---------------------------------------------------
	end of file
----------------------------------------------------*/