/*---------------------------------------------------
	communication.c (v1.00)
	
	ͨ�ų���
---------------------------------------------------*/	

#include "main.h"
#include "port.h"

#include "communication.h"
#include "Delay.h"
#include "uart.h"

/*------ private variable --------------------------*/
tByte myTxRxData[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//tByte IDkey11 = IDkey6^IDkey7^IDkey8^IDkey9^IDkey10;

/*------- Public variable declarations --------------------------*/
extern tByte receive_LV_count;		
extern bit receive_wire_flag;		
extern tByte one_receive_byte;		
extern tByte one_receive_byte_count;			
extern tByte receive_HV_count;		
extern tByte data_count;				
extern tByte received_data_buffer[7];		
extern bit receive_data_finished_flag;		

extern code tByte IDkey6;
extern code tByte IDkey7;
extern code tByte IDkey8;
extern code tByte IDkey9;
extern code tByte IDkey10;

/*----------------------------------------------------
	InitTransceiver()
	Initialise Transceiver
----------------------------------------------------*/
void InitTransceiver(void)
	{
	P10 = High;
	transmiter_EN = Close;
	receiver_EN = Open;	
	transmiter_power = 0; 		// High power mode
	}
	
/*----------------------------------------------------
	initsignal()
	
	��ʼ���źų��򣬽��ջ��ڽӽ����źŵ�ʱ����Ҫ��
	һ�δ�����ʹ���ջ��򿪣�����һ�γ����п����޷�ʶ��
	�����������൱�ڷϴ��룬ֻռ��ͨ��ʱ�䡣
----------------------------------------------------*/
void initsignal()
	{
	tByte k,k1;
	tByte mystartbuffer = 0xaa;
	for(k1 = 0; k1 < 3; k1++)
		{
		for(k=0;k<8;k++)
			{
			if((mystartbuffer&0x80) == 0x80)//Ϊ1
				{
				P10=0;
				Custom_Delay(46, 20);
//				Delay_5ms();
				}
			else
				{
				P10=0;
				Custom_Delay(46, 20);
//				Delay_5ms();
				}
			P10=1;
			mystartbuffer<<=1;
			Custom_Delay(40, 24);
//			Delay_5ms();
			}
		mystartbuffer=0xaa;
//		Delay_5ms();
//		Custom_Delay(23, 28);
		}
	P10=1;
	}

void initsignal_F(void)
	{
	tByte k,k1;
	tByte mystartbuffer = 0xaa;
	for(k1 = 0; k1 < 2; k1++)
		{
		for(k=0;k<6;k++)
			{
			if((mystartbuffer&0x80) == 0x80)//Ϊ1
				{
				P10=0;
				Custom_Delay(46, 20);
//				Delay_5ms();
				}
			else
				{
				P10=0;
				Custom_Delay(46, 20);
//				Delay_5ms();
				}
			P10=1;
			mystartbuffer<<=1;
			Custom_Delay(40, 24);
//			Delay_5ms();
			}
		mystartbuffer=0xaa;
//		Delay_5ms();
//		Custom_Delay(23, 28);
		}
	P10=1;
	}

/*--------------------------------------------------
	Send_Data()
	�����ݷ��ͳ�ȥ
--------------------------------------------------*/
void Send_Data(tByte x)
	{
	tByte i,n;
	for(i=0;i<3;i++)
		{
		for(n=0;n<8;n++)
			{
			if((myTxRxData[i]&0x80)==0x80)
				{
				P10=0;
				Custom_Delay(36, x);
				}
			else
				{
				P10=0;
				Custom_Delay(23, x);
				}
			P10=1;
			myTxRxData[i]<<=1;
			Custom_Delay(20, x);
			}
		}	
	}

/*--------------------------------------------------
	ComMode_Data()
	
	�������յ�����1�źź󣬻ᷴ��һ������1�źŸ�����
	�Ա�ʾ�����ڸ���������
---------------------------------------------------*/
void ComMode_Data(tByte ComMode, x)	
	{
	receiver_EN = 1;
	Delay(20);
	transmiter_EN = 0;
	myTxRxData[0] = CmdHead;
	myTxRxData[1] = MyAddress;
	myTxRxData[2] = ComMode;

	initsignal();
   Send_Data(x);
	
	transmiter_EN = 1;
	receiver_EN = 1;
	}

/*--------------------------------------------------
	ComMode_Data_WX()
	
	�������յ�����1�źź󣬻ᷴ��һ������1�źŸ�����
	�Ա�ʾ�����ڸ���������
---------------------------------------------------*/
void ComMode_Data_WX(tByte ComMode, x)	
	{
	receiver_EN = 1;
	Delay(20);
	transmiter_EN = 0;
	myTxRxData[0] = CmdHead;
	myTxRxData[1] = MyAddress;
	myTxRxData[2] = ComMode;

	initsignal();
   Send_Data(x);
	
	transmiter_EN = 0;
	receiver_EN = 0;
	TXD = 0;
	}

/*------------------------------------------------------------------
	UART��������
------------------------------------------------------------------*/
void UART_Send_Data(tByte command)
	{
   tByte ii = 0;
	open_tranceiver();
	myTxRxData[0] = IDkey6;
	myTxRxData[1] = IDkey7;
	myTxRxData[2] = IDkey8;
	myTxRxData[3] = IDkey9;
	myTxRxData[4] = IDkey10;
	myTxRxData[5] = command;
	
	initsignal();
	
	for(ii = 0; ii < 30; ii++)
		{
		SendNByte(myTxRxData, 6);
		Delay_10ms();		
		}

	close_tranceiver();
	}

/*------------------------------------------------------------------
	UART������������
------------------------------------------------------------------*/
void UART_Send_Data_F(tByte command)
	{
   tByte ii = 0;
	open_tranceiver();
	myTxRxData[0] = IDkey6;
	myTxRxData[1] = IDkey7;
	myTxRxData[2] = IDkey8;
	myTxRxData[3] = IDkey9;
	myTxRxData[4] = IDkey10;
	myTxRxData[5] = command;
	
	initsignal_F();
	
	SendNByte(myTxRxData, 6);

	close_tranceiver();	
	}

/*------------------------------------------------------------------
	open_tranceiver()
	�������
-------------------------------------------------------------------*/
void open_tranceiver(void)
	{
	#ifdef ID
	InitUART(BAUD1200);
	#endif
	
	receiver_EN = 1;
	transmiter_EN = 0;	
	}

/*------------------------------------------------------------------
	close_tranceiver()
	�������
-------------------------------------------------------------------*/
void close_tranceiver(void)
	{
	#ifdef F3
	transmiter_EN = 1;
	#endif
	
	receiver_EN = 0;
	
	#ifdef ID
	InitUART(BAUD9600);
	#endif
	}

/*------------------------------------------------------------------
	UART��������
------------------------------------------------------------------*/
void UART_Send_Data_match(void)
	{
	open_tranceiver();
	
	myTxRxData[0] = CmdHead;
	myTxRxData[1] = ComMode_1;
	myTxRxData[2] = IDkey6;
	myTxRxData[3] = IDkey7;
	myTxRxData[4] = IDkey8;
	myTxRxData[5] = IDkey9;
	myTxRxData[6] = IDkey10;
	
	initsignal();
	
	SendNByte(myTxRxData, 7);
   Delay_50ms();
	
	close_tranceiver();
	}

/*-----------------------------------------------------------------------------
	receive_byte()
	receive a byte program
-----------------------------------------------------------------------------*/
void receive_byte(void)
	{
	// P11 constantly HV, if detected a LV, judge it.
	if(P11 == 0)
		{
		// count the LV time, if more than 12ms, reset it.
		if(++receive_LV_count >= 120)
			{
			receive_LV_count = 0;
			}
		receive_wire_flag = 0;
		}
	// if P11 return to HV
	else
		{
		// and already have LV before, so we think it maybe a bit signal
		if(receive_wire_flag == 0)
			{
			// set the flag, to judge a bit only one time
			receive_wire_flag = 1;

			// judge the LV time, if 3.5ms < time < 8ms, we think it is a "0". time<3.5ms means a noise.
			if((receive_LV_count > 35)&&(receive_LV_count <= 80))	
				{
				// save "0" to one byte
				one_receive_byte <<= 1;
				one_receive_byte &= 0xfe;
				one_receive_byte_count++;
				receive_HV_count = 0;
				}
			// time > 8ms, means a "1"
			else if((receive_LV_count > 80))
				{
				// save "1" to one byte
				one_receive_byte <<= 1;
				one_receive_byte |= 0x01;
				one_receive_byte_count++;
				receive_HV_count = 0;
				}			
			else
				{
				// increase the count for HV
				receive_HV_count++;	
				}
         // reset LV count
			receive_LV_count = 0;
			}
		else
			{
			// judge whether HV count > 6ms, if yes, means a restart
			if(++receive_HV_count >= 60)
				{
				one_receive_byte_count = 0;
				receive_wire_flag = 1;
				data_count = 0;
				}		
			}
		}
	}

/*-----------------------------------------------------------------------------
	receive_byte_Lock()
	receive a byte program
-----------------------------------------------------------------------------*/
void receive_byte_Lock(void)
	{
	// P11 constantly HV, if detected a LV, judge it.
	if(receive_wire == 1)
		{
		// count the LV time, if more than 12ms, reset it.
		if(++receive_LV_count >= 120)
			{
			receive_LV_count = 0;
			}
		receive_wire_flag = 0;
		}
	// if P11 return to HV
	else
		{
		// and already have LV before, so we think it maybe a bit signal
		if(receive_wire_flag == 0)
			{
			// set the flag, to judge a bit only one time
			receive_wire_flag = 1;

			// judge the LV time, if 3.5ms < time < 8ms, we think it is a "0". time<3.5ms means a noise.
			if((receive_LV_count > 35)&&(receive_LV_count <= 80))	
				{
				// save "0" to one byte
				one_receive_byte <<= 1;
				one_receive_byte &= 0xfe;
				one_receive_byte_count++;
				receive_HV_count = 0;
				}
			// time > 8ms, means a "1"
			else if((receive_LV_count > 80))
				{
				// save "1" to one byte
				one_receive_byte <<= 1;
				one_receive_byte |= 0x01;
				one_receive_byte_count++;
				receive_HV_count = 0;
				}			
			else
				{
				// increase the count for HV
				receive_HV_count++;	
				}
         // reset LV count
			receive_LV_count = 0;
			}
		else
			{
			// judge whether HV count > 6ms, if yes, means a restart
			if(++receive_HV_count >= 60)
				{
				one_receive_byte_count = 0;
				receive_wire_flag = 1;
				data_count = 0;
				}		
			}
		}
	}

/*-----------------------------------------------------------------------------
	receive_word()
	receive a word program
-----------------------------------------------------------------------------*/
void receive_word(void)
	{
	// one byte have 8 bit, once receive a bit, the count increase, once it is 8, it means a byte received.
	if(one_receive_byte_count == 8)
		{
		one_receive_byte_count = 0;
		// assign one byte to buffer[i] 
		received_data_buffer[data_count] = one_receive_byte;
		
		// judge whether buffer[0] is CmdHead
		if((data_count == 0) && (received_data_buffer[0] == CmdHead))
			{
			data_count = 1;
			}
		// judge whether buffer[1] is MyAddress
		else if((data_count == 1) && (received_data_buffer[1] == MyAddress))
			{
			data_count = 2;
			}
		else if(data_count == 2)
			{
			receive_data_finished_flag = 1;
			data_count = 0;
			}
		else 
			{
			data_count = 0;
			}
		}
	}

/*--------------------------------------------------
	send_code_to_lock()
	
	�����뷢�͸����塣
---------------------------------------------------*/
void send_code_to_lock(tByte x, y)	
{
	unsigned char i,n;
	myTxRxData[0]=CmdHead;
	myTxRxData[1]=MyAddress;
	myTxRxData[2]=x;
/*	myTxRxData[3]=0x00;
	myTxRxData[4]=0x00;
	myTxRxData[5]=0x00;
	myTxRxData[6]=0x00;
*/
	for(i=0;i<3;i++)
	{
		for(n=0;n<8;n++)
		{
			if((myTxRxData[i]&0x80) == 0x80)//Ϊ1
			{
				MagentControl_2 = 0;
				Custom_Delay(36, y);
			}
			else//Ϊ0�����
			{
				MagentControl_2 = 0;
				Custom_Delay(23, y);
			}
			MagentControl_2 = 1;		//��̬Ϊ�ߵ�ƽ
			myTxRxData[i] <<= 1;
			Custom_Delay(15, y);
		}
	}
}

/*--------------------------------------------------
	send_code_to_lock_Taili()
	
	�����뷢�͸����塣
---------------------------------------------------*/
void send_code_to_lock_Taili(tByte x, y)	
{
	unsigned char i,n;
	myTxRxData[0]=CmdHead;
	myTxRxData[1]=MyAddress;
	myTxRxData[2]=x;
/*	myTxRxData[3]=0x00;
	myTxRxData[4]=0x00;
	myTxRxData[5]=0x00;
	myTxRxData[6]=0x00;
*/
	for(i=0;i<3;i++)
	{
		for(n=0;n<8;n++)
		{
			if((myTxRxData[i]&0x80) == 0x80)//Ϊ1
			{
				lock_code = 0;
				Custom_Delay(36, y);
			}
			else//Ϊ0�����
			{
				lock_code = 0;
				Custom_Delay(23, y);
			}
			lock_code = 1;		//��̬Ϊ�ߵ�ƽ
			myTxRxData[i] <<= 1;
			Custom_Delay(15, y);
		}
	}
}

/*---------------------------------------------------
	end of file
----------------------------------------------------*/