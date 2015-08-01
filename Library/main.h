/*---------------------------------------------------
	Main.h (v1.00)
	
	'project header' (see chap 5)
---------------------------------------------------*/

#ifndef _MAIN_H
#define _MAIN_H

//--------header file--------------------------------

// Program version
//#define Taili, General, Tailing			
// Define customer, such as Taili, Shengbaolong, Wangpai, Tailing, General
#define General

// Define PCB board, such as Z2, Z3.
#define Z2

// Define salve model, F2, F3.
#define F2

// Define model.
// ID, WX, 
#define WX

// Define the Lock model.
//#define Suidongzha, Guxingzha
#define Guxingzha

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
#define BAUD9600 9600
#define BAUD1200 1200

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