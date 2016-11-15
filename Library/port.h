/*----------------------------------------------------
	port.h (v1.00)
	port-old.h
	'port header' (see chap 5) for DDC-Z
----------------------------------------------------*/

#ifndef _PORT_H
#define _PORT_H

#include "main.h"

/*----------------------------------------------------
	�����˿ڶ���
-----------------------------------------------------*/

// P2.5，PIN16，钥匙输入，1为电动车打开钥匙，0为电动车关闭钥匙
sbit key_rotate = P2^5;


// P0.2, PIN25, 传感器输入
sbit sensor_detect = P0^2;
// P0.2, PIN25, horizontal sensor input
sbit horizontal_sensor = P0^2; 
// P2.6, PIN27, 滚珠传感器
sbit the3rd_sendor = P2^6;


// P1.4，PIN10，功放开关输出，1打开功放，0关闭功放
sbit voice_EN = P1^4;

// P2.0，P2.1，PIN1，PIN2，语音输出
sbit SC_RST=P2^0;
sbit SC_DATA=P2^1;
// P0.7, PIN19， 语音状态
sbit SC_status = P0^7;

// P0.5, PIN22, Control the External motor, it is vdd of Hall signal actually.
sbit Externalmotor = P0^5;

// P0.1, PIN26, 行车自锁输入
sbit wheeled_rotate = P0^1;

// P0.6, PIN20, ֧ˤɫ
sbit battery_input = P0^6;

// P0.0, PIN3, ̸֧ܺ̀ˤԶ
sbit Generator_lock = P0^0;

// P2.7, PIN28, 用来作为密码匹配测试用。
sbit Match_wire = P2^7;

// 

sbit testport = P2^4;

/*---------DDC-S -----------------------------------*/
// P3.0, PIN9, ���տ�������
sbit receive_wire = P3^0;
sbit receive_wire2 = P2^4;

// P2.2, PIN13, P2.3, PIN14, 开锁关锁的信号
sbit MagentControl_1 = P2^2;
sbit MagentControl_2 = P2^3;

/*----------------------------------------------------
	�����˿ڶ���
-----------------------------------------------------*/
// P2.6, PIN27, ������������
sbit toggle_button = P2^6;

// P0.1, PIN26, ������ť
sbit press_close_button = P0^1;

// P0.0, PIN3, ������ť
sbit press_open_button = P0^0;
sbit match_button = P0^0;

// P0.2, PIN25, ����ģ���Դ����
sbit transceiver_power_enable = P0^2;

#endif
											
/*---------------------------------------------------
	end of file
----------------------------------------------------*/