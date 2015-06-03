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
	�ӳٳ���
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
	�ӳٳ���
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
	
	�ӳ�ԼΪ1ms���ӳ���
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
	
	�ӳ�ԼΪ1us���ӳ���
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
	��������ӳٳ���Ҫ���������Թ������ж�
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
		// �����������Ƿ���ת�����Ϊ1���������������
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
