//************************************************************************************************************************
// MqttDeviceONDomoticzHandler.cpp
// Version 1.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#include <Stream.h>
#include <StreamString.h>

#include "MqttDeviceONDomoticzHandler.h"

#include "Settings.h"



// Todo :
// - Ajouter les paramètres du serveur mqtt (Ip, Port..) a la page du wifimanager
// https://github.com/tzapu/WiFiManager/blob/master/examples/AutoConnectWithFSParameters/AutoConnectWithFSParameters.ino


namespace wifix {

SINGLETON_IMPL (MqttDeviceONDomoticzPublisher)

//========================================================================================================================
//
//========================================================================================================================
size_t MqttDeviceONDomoticzPublisher :: publishDeviceONState () {

	return publishSwitchStatus (REMOTE_FREEBOX_PLAYER_SWITCH_IDX, true);
}


/************************************************************************************************************************/


SINGLETON_IMPL (MqttDeviceONDomoticzSubscriber)


//========================================================================================================================
//
//========================================================================================================================
bool MqttDeviceONDomoticzSubscriber :: parseJsonObj	(const JsonObject& jsonArg) {

	// Publish received :
	//  -topic = domoticz/out
	//  -payload = {
	//		"Battery":255,
	//		"LastUpdate":"2025-01-07 10:57:06",
	//		"RSSI":12,
	//		"description":"",
	//		"dtype":"Light/Switch",
	//		"hwid":"7",
	//		"id":"00014069",
	//		"idx":25,
	//		"name":"Freebox Player",
	//		"nvalue":1,
	//		"o[ESP13901933] rg_hwid":"7",
	//		"stype":"Switch",
	//		"svalue1":"0",
	//		"switchType":"On/Off","unit":1
	// }

	// décode le message - decode payload message

	if (jsonArg [PAYLOAD_IDX].success()) {

		int idx = jsonArg [PAYLOAD_IDX];

		// Handle message
		if (idx == REMOTE_FREEBOX_PLAYER_SWITCH_IDX) {

			if (jsonArg [PAYLOAD_NVALUE].success()) {

				// Nothing to do ..
				return true;
			}
		}
	}

	return false;
}

}