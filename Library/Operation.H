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
void ENsensor_After_Close(void);
void SelfLearn_Reset(void);
void Fell_Alarm_to_Slave(void);
void Raise_Alarm_to_Slave(void);
void Batstolen_Alarm_to_Slave(void);
void Disable_sensor_after_IDcert(void);
void Reset_after_wirebroken(void);
void Reset_after_stolen_alarming(void);
void Ensensor_after_slave_away(void);
void Detect_selflearn_action(void);
void Detect_open_action(void);
void Detect_close_action(void);
void Detect_vibration(void);
void Detect_wheel_moving(void);

#endif
											
/*---------------------------------------------------
	end of file
----------------------------------------------------*/