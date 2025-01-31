//************************************************************************************************************************
// Settings.h
// Version 1.0 October, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

//------------------------------------------------------------------------------
//

#define USING_WIFI
#define USING_ETH

// Add MQTT subscriber & publisherq
#define USING_DOMOTICZ_MQTT
//#define USING_AWS_MQTT

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

#define MQTT_DOMOTICZ_ENDPOINT			"192.168.1.138"		// IP Broker MQTT
#define MQTT_DOMOTICZ_PORT				1883				// Port Broker MQTT

// Relay component
#define MQTT_DOMOTICZ_RELAY_SWITCH_IDX 	44
#define MQTT_DOMOTICZ_LOG_IDX			17
