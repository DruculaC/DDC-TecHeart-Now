/*---------------------------------------------------
	Delay.c (v1.00)
	
	Delay functions
---------------------------------------------------*/	

#include "main.h"
#include "port.h"

#include "Delay.h"

tWord Timeout_loop = 0;

/*------------------------------------------------------------------*/
void Delay_ms(tWord xtime)
   {
   Timeout_loop = xtime;

   // Simple loop timeout...
   while (++Timeout_loop != 0);
   }


/*------------------------------------------------------------------*/
void Delay_1ms(void)
   {
   Timeout_loop = LOOP_TIMEOUT_INIT_001ms;

   // Simple loop timeout...
   while (++Timeout_loop != 0);
   }
/*------------------------------------------------------------------*/
void Delay_5ms(void)
   {
   Timeout_loop = LOOP_TIMEOUT_INIT_005ms;

   // Simple loop timeout...
   while (++Timeout_loop != 0);
   }
/*------------------------------------------------------------------*/
void Delay_10ms(void)
   {
   Timeout_loop = LOOP_TIMEOUT_INIT_010ms;

   // Simple loop timeout...
   while (++Timeout_loop != 0);
   }
/*------------------------------------------------------------------*/
void Delay_12ms(void)
   {
   Timeout_loop = LOOP_TIMEOUT_INIT_012ms;

   // Simple loop timeout...
   while (++Timeout_loop != 0);
   }

/*------------------------------------------------------------------*/
void Delay_50ms(void)
   {
   Timeout_loop = LOOP_TIMEOUT_INIT_050ms;

   // Simple loop timeout...
   while (++Timeout_loop != 0);
   }

/*------------------------------------------------------------------*/
void Delay_100ms(void)
   {
   Timeout_loop = LOOP_TIMEOUT_INIT_100ms;

   // Simple loop timeout...
   while (++Timeout_loop != 0);
   }

/*------------------------------------------------------------------*/
void Delay_500ms(void)
   {
   Timeout_loop = LOOP_TIMEOUT_INIT_500ms;

   // Simple loop timeout...
   while (++Timeout_loop != 0);
   }

/*---------------------------------------------------
	Delay()
	ÑÓ³Ù³ÌÐò
---------------------------------------------------*/

void Delay(tByte n)
	{
	tWord x;
	while(n--)
		{
		x=5000;
		while(x--);
		}
	}
	
/*---------------------------------------------------
	end of file
----------------------------------------------------*/
