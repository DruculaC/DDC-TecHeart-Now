/*---------------------------------------------------
	Delay.c (v1.00)
	
	Delay functions
---------------------------------------------------*/	

#include "main.h"
#include "port.h"

#include "Delay.h"

tWord Timeout_loop = 0;

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
	延迟程序
--------------------------------------------------*/

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
	Custom_Delay()
	延迟程序
--------------------------------------------------*/
void Custom_Delay(tByte t, m)
	{
	tWord i;
	tByte j;
	for(i = 0; i < t; i++)		
	for(j = 0; j < m; j++);
	}
	
/*--------------------------------------------------
	delay_ms()
	
	延迟约为1ms的子程序
----------------------------------------------------*/
void delay_ms(tWord count)
	{
	tWord i, j;
	for(i=0;i<count;i++)
		{	
		for(j=0;j<100;j++);
		}
	}

/*--------------------------------------------------
	delay_us()
	
	延迟约为1us的子程序
----------------------------------------------------*/

void delay_us(tWord count1)
	{
	tWord i;
	tByte j;
	for(i=0;i<count1;i++)
		{
		for(j=0;j<4;j++);
		}
	}

/*----------------------------------------------------
	Magnet_Delay()
	电磁铁的延迟程序，要在里面加入对过流的判断
-----------------------------------------------------*/
void Magnet_Delay(tWord x, y)
	{
	tByte i;
	tWord j;                                                                                                        
	for(i = 0; i < 50; i++)
		{
		for(j = 0; j< x; j++);
		}
	for(i = 0; i < 50; i++)
		{
		// 检测过流保护是否跳转，如果为1，则表明电流过大
		if(magnet_overcurrent == 0)
			{
			for(j = 0; j< y; j++);
			}
		else
			{
			return;
			}
		}
	}


/*---------------------------------------------------
	end of file
----------------------------------------------------*/
