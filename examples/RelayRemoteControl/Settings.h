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
#	define RELAY_PIN			PORT0B
#elif defined(ARDUINO_ESP8266_NODEMCU_ESP12E)
#	define RELAY_PIN			D1
#elif defined(ARDUINO_WT32_ETH01)
#	define RELAY_PIN			IO32
#endif

//------------------------------------------------------------------------------
// Domoticz MQTT
//

#define MQTT_SERVER_IP				"192.168.1.138"		// IP Broker MQTT
#define MQTT_SERVER_PORT			1883				// Port Broker MQTT

// Relay component
#define REMOTE_RELAY_SWITCH_IDX 	44
#define REMOTE_MQTT_LOG_IDX			17
