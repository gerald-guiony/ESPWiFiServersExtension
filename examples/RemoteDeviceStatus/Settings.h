//************************************************************************************************************************
// Settings.h
// Version 1.0 October, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once


//------------------------------------------------------------------------------
//

// Add Wifi request handlers
#define USING_WIFI

// Add MQTT subscriber & publisher
#define USING_MQTT


//------------------------------------------------------------------------------
// Pins
//

#include <EspBoardDefs.h>



#ifdef ARDUINO_ESP8266_WIO_NODE

#elif defined(ARDUINO_ESP8266_NODEMCU_ESP12E)

#elif defined(ESP32)

#else

#endif


//------------------------------------------------------------------------------
// Domoticz MQTT
//

#define MQTT_SERVER_IP						"192.168.1.138"		// IP Broker MQTT
#define MQTT_SERVER_PORT					1883				// Port Broker MQTT

// Switch in Domoticz
#define REMOTE_FREEBOX_PLAYER_SWITCH_IDX	25
