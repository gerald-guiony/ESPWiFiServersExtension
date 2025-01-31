//************************************************************************************************************************
// MqttRelayDomoticzHandler.cpp
// Version 1.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#include <Stream.h>
#include <StreamString.h>

#include <Switches/BasicRelay.h>

#include "MqttRelayDomoticzHandler.h"

#include "Settings.h"


extern std::vector <BasicRelay> relays;


namespace wifix {

// Todo :
// - Ajouter les paramètres du serveur mqtt (Ip, Port..) a la page du wifimanager
// https://github.com/tzapu/WiFiManager/blob/master/examples/AutoConnectWithFSParameters/AutoConnectWithFSParameters.ino


SINGLETON_IMPL (MqttRelayDomoticzPublisher)

//========================================================================================================================
//
//========================================================================================================================
size_t MqttRelayDomoticzPublisher :: publishStatutMessage (const String & msg) {

	static const String msgHeader = "[" + EspBoard::getDeviceName() + "] ";
	String custom_msg = msgHeader + msg;

	return publishMessage (MQTT_DOMOTICZ_LOG_IDX, custom_msg);
}


/************************************************************************************************************************/


SINGLETON_IMPL (MqttRelayDomoticzSubscriber)

//========================================================================================================================
//
//========================================================================================================================
void MqttRelayDomoticzSubscriber :: setup (AsyncMqttClient * asyncMqttClient) {

	_idx = MQTT_DOMOTICZ_RELAY_SWITCH_IDX;
	MqttDomoticzSubscriberIdx :: setup (asyncMqttClient);
}

//========================================================================================================================
//
//========================================================================================================================
bool MqttRelayDomoticzSubscriber :: onMqttMsgReceivedIdx (const JsonObject& jsonArg) {

	// domoticz/out {
		// "Battery" : 255,
		// "RSSI" : 12,
		// "description" : "",
		// "dtype" : "Light/Switch",
		// "id" : "00014051",
		// "idx" : 255,
		// "name" : "Kitchen light (MQTT)",
		// "nvalue" : 1,
		// "stype" : "Switch",
		// "svalue1" : "0",
		// "switchType" : "On/Off",
		// "unit" : 1
	// }

	// décode le message - decode payload message
	// Here we know that the _idx member value matches the idx of the domoticz device

	if (jsonArg [PAYLOAD_NVALUE].success()) {

		bool close = (jsonArg [PAYLOAD_NVALUE] == 1);

		if (close)
			relays[0].close();
		else
			relays[0].open();

		// Response
		I(MqttRelayDomoticzPublisher).publishStatutMessage (close ? "Relay closed":"Relay opened");

		return true;
	}

	return false;
}

}