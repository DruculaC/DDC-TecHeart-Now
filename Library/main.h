/*---------------------------------------------------
	Main.h (v1.00)
	
	'project header' (see chap 5)
---------------------------------------------------*/

#ifndef _MAIN_H
#define _MAIN_H

//--------header file--------------------------------

// Whether Debug or Normal
// #define Debug

// Locktest
//#define Locktest

// Program version
//#define Taili, General, Tailing			
#define Shengbaolong

// Define customer, such as Taili, Shengbaolong, Wangpai, Tailing, General, Xinri
//#define General

// define speech or not
#define voice

// 新锁（时间短）或者旧锁（时间长）
#define Old_lock

// Define PCB board, such as Z2, Z3.
#define Z2

// 定义语音芯片为C4
// Define Speech_C4

// Define slave model, F2, F3.
// #define F3

// Define model.
// ID, WX,
#define ID

#define BroadcastBattery

// Define the Lock model.
//#define Suidongzha, Guxingzha
#define Guxingzha

// Define battery life, Kilometer_110, Kilometer_65
// #define Kilometer_65

// Define voice version, 4031C's version: IC1603_4031C3, IC1512_4031C1, IC1506_4031C1, ICold_4031C1.
#define IC1603_4031C3

// Define PCB version, such as PCB_old, PCB_1606
#define PCB_1606

// Define Motor battery parameter and whether main board with battery.
// Withbattery_60V10A, Withbattery_48V10A
// Withoutbattery_48V10A, Withoutbattery_60V10A
// #define Withoutbattery

// Must include the appropriate microcontroller header file here
#include "N79E81x.h"
#include <intrins.h>

// oscillator / resonator frequency (in Hz)
#define OSC_FREQ (11059200UL)

// number of oscillations per instruction (12, etc)
// 12 - original 8051/8052 and numerous modern versions
// 6 - various Infineon and Philips devices, etc.
// 4 - Dallas 320, 520 etc.
// 2 - Dallas 420, etc
#define OSC_PER_INST (12)

// baud rate defined
#define BAUD9600 (9600)

#define BAUD1200 (1200)

//---------------------------------------------------
// should not need to edit the sections below
//---------------------------------------------------

// typedefs
typedef unsigned char tByte;
typedef unsigned int tWord;
typedef unsigned long tLong;

// Misc defines
#ifndef True
#define False 0
#define True (!False)
#endif

#ifndef Open
#define Close 1
#define Open (!Close)
#endif

#ifndef High
#define Low 1
#define High (!Low)
#endif

#define RETURN_NORMAL (bit) 0
#define RETURN_ERROR (bit) 1

// Interrupts
#define interrupt_timer_0_overflow 1
#define interrupt_timer_1_overflow 3
#define interrupt_timer_2_overflow 5
#define Interrupt_UART_Rx_Tx 4
#define Interrupt_CAN_c515c 17

// Error codes
#define ERROR_SCH_TOO_MANY_TASKS (1)
#define ERROR_SCH_CANNOT_DELETE_TASK (2)

#define ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK (0xAA)
#define ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER (0xAA)

#define ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START (0xA0)
#define ERROR_SCH_LOST_SLAVE (0x80)

#define ERROR_SCH_CAN_BUS_ERROR (6)

#define ERROR_I2C_WRITE_BYTE (13)
#define ERROR_I2C_READ_BYTE (14)
#define ERROR_I2C_WRITE_BYTE_AT24C64 (11)
#define ERROR_I2C_READ_BYTE_AT24C64 (12)
#define ERROR_I2C_DS1621 (15)

#define ERROR_USART_TI (21)
#define ERROR_USART_WRITE_CHAR (22)

#define ERROR_SPI_EXCHANGE_BYTES_TIMEOUT (31)
#define ERROR_SPI_X25_TIMEOUT (32)
#define ERROR_SPI_MAX1110_TIMEOUT (33)

#define ERROR_ADC_MAX150_TIMEOUT (44)
#endif

/*---------------------------------------------------
	end of file
----------------------------------------------------*/