//************************************************************************************************************************
// Settings.h
// Version 1.0 October, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#define TIME_AFTER_POWER_ON_FOR_OTA_UPDATE_MS	300000			// 300s == 5 minutes
#define TIME_FOR_REFRESH_DEVICE_ON_STATUS_MS	300000			// every 300s == 5 minutes


//------------------------------------------------------------------------------
// Domoticz MQTT
//

#define MQTT_SERVER_IP						"192.168.1.138"		// IP Broker MQTT
#define MQTT_SERVER_PORT					1883				// Port Broker MQTT

// Switch in Domoticz
#define REMOTE_FREEBOX_PLAYER_SWITCH_IDX	25
