//************************************************************************************************************************
// MqttDeviceONDomoticzHandler.cpp
// Version 1.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************

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

	_isDeviceOnPublished = true;
	return publishSwitchStatus (MQTT_DOMOTICZ_FREEBOX_PLAYER_SWITCH_IDX, true);
}


/************************************************************************************************************************/

SINGLETON_IMPL (MqttDeviceONDomoticzSubscriber)

//========================================================================================================================
//
//========================================================================================================================
void MqttDeviceONDomoticzSubscriber :: setup (AsyncMqttClient * asyncMqttClient) {

	_idx = MQTT_DOMOTICZ_FREEBOX_PLAYER_SWITCH_IDX;
	MqttDomoticzSubscriberIdx :: setup (asyncMqttClient);
}

//========================================================================================================================
//
//========================================================================================================================
bool MqttDeviceONDomoticzSubscriber :: onTopicIdxReceived	(const JsonObject& jsonArg) {

	// décode le message - decode payload message
	// Here we know that the _idx member value matches the idx of the domoticz device

	if (jsonArg [PAYLOAD_NVALUE].success()) {

		// Nothing to do ..
		return true;
	}

	return false;
}

}