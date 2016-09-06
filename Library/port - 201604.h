/*----------------------------------------------------
	port.h (v1.00)
	port-201604
	'port header' (see chap 5) for DDC-Z
----------------------------------------------------*/

#ifndef _PORT_H
#define _PORT_H

#include "main.h"

/*----------------------------------------------------
	主机端口定义
-----------------------------------------------------*/

// P2.5，PIN16，连接到钥匙转动，钥匙转开时，此pin为1；钥匙关闭时，此pin为0
sbit key_rotate = P2^5;			

// P2.6，PIN27，发射机的功率控制。1表示小功率，0表示大功率。大功率发射距离远。
sbit transmiter_power = P2^6;

// P2.7，PIN28，发射机使能端口。0表示开，1表示关
sbit transmiter_EN = P2^7;


// P0.0, PIN3, horizontal sensor input
sbit sensor_detect = P0^0;

// P2.2，P2.3，分别为PIN13，PIN14，电磁铁,  平时值为00，开锁用10，关锁有01，然后都恢复到00
sbit MagentControl_1 = P2^2;
sbit MagentControl_2 = P2^3;

// P0.4，PIN23，抬起传感器的检测
sbit raised_sensor_detect = P0^4;

// P0.3，PIN24，倒地传感器的检测
sbit fell_sensor_detect = P0^3;

// P1.4，PIN10，功放开关控制，1为打开功放，0为关闭功放
sbit voice_EN = P1^4;

// P2.0，P2.1分别为PIN1，PIN2，对应语音芯片的复位和数据管脚
sbit SC_RST=P2^0;
sbit SC_DATA=P2^1;

// P0.7，PIN19，传感器的检测结果，通过此PIN传给MCU，然后MCU判断
sbit horizontal_sensor = P0^7; 
// 
sbit the3rd_sendor = P2^6;

// P1.3, PIN11, receiver enable，0是打开，1是关闭
sbit receiver_EN = P1^3;

// P0.5, PIN22, Control the External motor, it is vdd of Hall signal actually.
sbit Externalmotor = P0^5;

// P0.2, PIN25, 电磁铁状态记忆输入
sbit magnet_status = P0^2;

// P0.1, PIN26, 断线报警检测管脚，常态为1，当为0时，则表示有人剪线。
sbit wire_broken = P0^1;
//sbit wire_broken = P1^7;


sbit lock_code = P0^1;

// P1.2, PIN12, 电磁铁过流检测
sbit magnet_overcurrent = P1^2;
sbit ElecMotor_overcurrent = P1^2;

// P0.2, PIN25, 车轮转动检测
sbit wheeled_rotate = P0^2;

// P2.4, PIN15, 用于锁体程序，控制锁的开关
sbit Lock_EN = P2^4;

// P2.2, PIN13, 用于F3，控制振动马达
sbit Moto_EN = P2^2;

// P3.0, PIN9, 接收开锁密码
sbit receive_wire = P3^0;

// P1.0, PIN18, 发射信号输出线
sbit transmit_wire = P1^0;

// P0.6, PIN20, 电量输入
sbit battery_input = P0^6;

// P1.6, PIN5, 电机锁死输出
sbit Generator_lock = P1^6;

/*----------------------------------------------------
	附机端口定义
-----------------------------------------------------*/
// P2.6, PIN27, 拨动开关输入
sbit toggle_button = P2^6;

// P0.4, PIN23, 关锁按钮
sbit wakeup_button = P0^4;

// P0.0, PIN3, 开锁按钮
sbit press_open_button = P0^0;
sbit match_button = P0^0;

// P2.7, PIN28, 无线模块电源控制
sbit transceiver_power_enable = P2^7;

#endif
											
/*---------------------------------------------------
	end of file
----------------------------------------------------*/