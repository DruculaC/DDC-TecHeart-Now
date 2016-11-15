/*----------------------------------------------------
	port.h (v1.00)
	port-old.h
	'port header' (see chap 5) for DDC-Z
----------------------------------------------------*/

#ifndef _PORT_H
#define _PORT_H

#include "main.h"

/*----------------------------------------------------
	Ö÷»ú¶Ë¿Ú¶¨Òå
-----------------------------------------------------*/

// P2.5ï¼ŒPIN16ï¼Œé’¥åŒ™è¾“å…¥ï¼Œ1ä¸ºç”µåŠ¨è½¦æ‰“å¼€é’¥åŒ™ï¼Œ0ä¸ºç”µåŠ¨è½¦å…³é—­é’¥åŒ™
sbit key_rotate = P2^5;


// P0.2, PIN25, ä¼ æ„Ÿå™¨è¾“å…¥
sbit sensor_detect = P0^2;
// P0.2, PIN25, horizontal sensor input
sbit horizontal_sensor = P0^2; 
// P2.6, PIN27, æ»šç ä¼ æ„Ÿå™¨
sbit the3rd_sendor = P2^6;


// P1.4ï¼ŒPIN10ï¼ŒåŠŸæ”¾å¼€å…³è¾“å‡ºï¼Œ1æ‰“å¼€åŠŸæ”¾ï¼Œ0å…³é—­åŠŸæ”¾
sbit voice_EN = P1^4;

// P2.0ï¼ŒP2.1ï¼ŒPIN1ï¼ŒPIN2ï¼Œè¯­éŸ³è¾“å‡º
sbit SC_RST=P2^0;
sbit SC_DATA=P2^1;
// P0.7, PIN19ï¼Œ è¯­éŸ³çŠ¶æ€
sbit SC_status = P0^7;

// P0.5, PIN22, Control the External motor, it is vdd of Hall signal actually.
sbit Externalmotor = P0^5;

// P0.1, PIN26, è¡Œè½¦è‡ªé”è¾“å…¥
sbit wheeled_rotate = P0^1;

// P0.6, PIN20, Ö§Ë¤É«
sbit battery_input = P0^6;

// P0.0, PIN3, Ö§ÜºÌ¸Ì€Ë¤Ô¶
sbit Generator_lock = P0^0;

// P2.7, PIN28, ç”¨æ¥ä½œä¸ºå¯†ç åŒ¹é…æµ‹è¯•ç”¨ã€‚
sbit Match_wire = P2^7;

// 

sbit testport = P2^4;

/*---------DDC-S -----------------------------------*/
// P3.0, PIN9, ½ÓÊÕ¿ªËøÃÜÂë
sbit receive_wire = P3^0;
sbit receive_wire2 = P2^4;

// P2.2, PIN13, P2.3, PIN14, å¼€é”å…³é”çš„ä¿¡å·
sbit MagentControl_1 = P2^2;
sbit MagentControl_2 = P2^3;

/*----------------------------------------------------
	¸½»ú¶Ë¿Ú¶¨Òå
-----------------------------------------------------*/
// P2.6, PIN27, ²¦¶¯¿ª¹ØÊäÈë
sbit toggle_button = P2^6;

// P0.1, PIN26, ¹ØËø°´Å¥
sbit press_close_button = P0^1;

// P0.0, PIN3, ¿ªËø°´Å¥
sbit press_open_button = P0^0;
sbit match_button = P0^0;

// P0.2, PIN25, ÎŞÏßÄ£¿éµçÔ´¿ØÖÆ
sbit transceiver_power_enable = P0^2;

#endif
											
/*---------------------------------------------------
	end of file
----------------------------------------------------*/