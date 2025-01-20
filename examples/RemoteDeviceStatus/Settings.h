//************************************************************************************************************************
// Settings.h
// Version 1.0 October, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#define TIME_WITHOUT_DEEPSLEEP_ms			300000				// 300s == 5 minutes
#define DEVICE_ON_STATE_PUBLISH_PERIOD_ms	300000				// every 300s == 5 minutes


//------------------------------------------------------------------------------
// Domoticz MQTT
//

#define MQTT_SERVER_IP						"192.168.1.138"		// IP Broker MQTT
#define MQTT_SERVER_PORT					1883				// Port Broker MQTT

// Switch in Domoticz
#define REMOTE_FREEBOX_PLAYER_SWITCH_IDX	25
