/*---------------------------------------------------
	Timer.c (v1.00)
	
	Timer functions
---------------------------------------------------*/	

#include "main.h"
#include "port.h"

#include "Timer.h"
#include "communication.h"
#include "voice.h"
#include "Delay.h"
#include "Battery.h"
#include "AD.h"
#include "Other.h"
#include "operation.h"
#include "hSch51.h"
#include "Elecmotor.h"

// ------ Public variable declarations -----------------------------
extern sTaskH hSCH_tasks_G[hSCH_MAX_TASKS];
extern tWord timer0_count;			
extern bit CW_flag;

// ------ Private variable definitions -----------------------------
tByte timer0_8H, timer0_8L, timer1_8H, timer1_8L;		// register value of timer0 and timer1, caculated from InitTimer()


/*--------------------------------------------------
	InitTimer()
	
	初始化定时器T0和T1
---------------------------------------------------*/

void InitTimer(const tByte Tick_ms_T0, Tick_us_T1)
	{
	tLong Inc_T0, Inc_T1;
	tWord timer0_16, timer1_16;
	
	//计算Timer0的寄存器值
	Inc_T0 = (tLong)Tick_ms_T0 * (OSC_FREQ/1000) / (tLong)OSC_PER_INST;
	timer0_16 = (tWord) (65536UL - Inc_T0);
	timer0_8H = (tByte) (timer0_16 / 256);
	timer0_8L = (tByte) (timer0_16 % 256);
	
	//计算Timer1的寄存器值
	Inc_T1 = (tLong)Tick_us_T1 * (OSC_FREQ/1000000) / (tLong)OSC_PER_INST;
	timer1_16 = (tWord) (65536UL - Inc_T1);
	timer1_8H = (tByte) (timer1_16 / 256);
	timer1_8L = (tByte) (timer1_16 % 256);
	
	TMOD = 0x11;
	
	TH0 = timer0_8H;
	TL0 = timer0_8L;
	TH1 = timer1_8H;
	TL1 = timer1_8L;

	ET0 = 1;
	TR0 = 1;
	ET1 = 1;
	TR1 = 1;
	PT1 = 1;			
	EA = 1;
	}

/*-----------------------------------------------------------------
	初始化T0，在初始化了串口之后
-----------------------------------------------------------------*/
void InitT0(const tByte Tick_ms_T0)
	{
	tLong Inc_T0;
	tWord timer0_16;
   tByte i;
  
	for (i = 0; i < hSCH_MAX_TASKS; i++) 
      {
      hSCH_Delete_Task(i);
      }
	
	//计算Timer0的寄存器值
	Inc_T0 = (tLong)Tick_ms_T0 * (OSC_FREQ/1000) / (tLong)OSC_PER_INST;
	timer0_16 = (tWord) (65536UL - Inc_T0);
	timer0_8H = (tByte) (timer0_16 / 256);
	timer0_8L = (tByte) (timer0_16 % 256);
	
   TMOD &= 0xF0; 	// Clear all T0 bits (T1 left unchanged)
   TMOD |= 0x01; 	// Set required T0 bits (T1 left unchanged) 
	
	hSCH_Manual_Timer0_Reload();
   // Interrupt Timer 0 enabled
   ET0  = 1;
	}

/*------------------------------------------------------------------*-

  hSCH_Start()

  Starts the scheduler, by enabling interrupts.

  NOTE: Usually called after all regular tasks are added,
  to keep the tasks synchronised.

  NOTE: ONLY THE SCHEDULER INTERRUPT SHOULD BE ENABLED!!! 
 
-*------------------------------------------------------------------*/
void hSCH_Start(void) 
   {
   EA = 1;
   }

/*---------------------------------------------------------------------
	sEos_Go_To_Sleep()
	将MCU进入休眠状态
----------------------------------------------------------------------*/
void sEOS_Go_To_Sleep(void)
   {
   PCON |= 0x01;    // Enter idle mode (generic 8051 version)
   }


/*------------------------------------------------------------------*-

  hSCH_Manual_Timer0_Reload()

  This scheduler uses a (manually reloaded) 16-bit timer.
  The manual reload means that all timings are approximate. 
  THIS SCHEDULER IS NOT SUITABLE FOR APPLICATIONS WHERE
  ACCURATE TIMING IS REQUIRED!!!
  Timer reload is carried out in this function. 
 
-*------------------------------------------------------------------*/
void hSCH_Manual_Timer0_Reload()
   {
   // Stop Timer 0
   TR0 = 0;

   // 8051, 12 MHz
   // The Timer 0 resolution is 1.000 祍
   // We set the timer at 64336 to generate interrupt after 1 ms
   // -> we are generating timer ticks at ~1 ms intervals
   TL0  = timer0_8L;
   TH0  = timer0_8H;	

   //  Start Timer 0
   TR0  = 1;
   }

/*------------------------------------------------------------------
	timerT0()
	operation every ticket.

void timer0() interrupt interrupt_timer_0_overflow
	{
   tByte Index;

   // Reload the timer
   hSCH_Manual_Timer0_Reload();
   // NOTE: calculations are in *TICKS* (not milliseconds)
   for (Index = 0; Index < hSCH_MAX_TASKS; Index++)
      {
      // Check if there is a task at this location
      if (hSCH_tasks_G[Index].pTask)
         {
         if (hSCH_tasks_G[Index].Delay == 0)
            {
            // The task is due to run
            hSCH_tasks_G[Index].RunMe += 1;  // Inc. the 'Run Me' flag

            if (hSCH_tasks_G[Index].Period)
               {
               // Schedule periodic tasks to run again
               hSCH_tasks_G[Index].Delay = hSCH_tasks_G[Index].Period;
               }
            }
         else
            {
            // Not yet ready to run: just decrement the delay 
            hSCH_tasks_G[Index].Delay -= 1;
            }
         }         
      }
	timer0_count += 1;
	if(timer0_count > 200)
		{
		TXD = ~TXD;
		timer0_count = 0;
		if(CW_flag == 0)
			{
			CW_flag = 1;
			ElecMotor_CW();
			}
		else
			{
			CW_flag = 0;
			ElecMotor_ACW();			
			}
		}
	
 	}
--------------------------------------------------------------------*/
	
/*---------------------------------------------------
	end of file
----------------------------------------------------*/