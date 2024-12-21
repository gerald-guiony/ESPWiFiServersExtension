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



// Todo :
// - Ajouter les paramètres du serveur mqtt (Ip, Port..) a la page du wifimanager
// https://github.com/tzapu/WiFiManager/blob/master/examples/AutoConnectWithFSParameters/AutoConnectWithFSParameters.ino



SINGLETON_IMPL (MqttRelayDomoticzPublisher)


//========================================================================================================================
//
//========================================================================================================================
size_t MqttRelayDomoticzPublisher :: publishStatutMessage (const String & msg) {

	static const String msgHeader = "[" + getChipName() + "] ";
	String custom_msg = msgHeader + msg;

	return publishMessage (REMOTE_MQTT_LOG_IDX, custom_msg);
}


/************************************************************************************************************************/


SINGLETON_IMPL (MqttRelayDomoticzSubscriber)


//========================================================================================================================
//
//========================================================================================================================
bool MqttRelayDomoticzSubscriber :: parseJsonObj	(const JsonObject& jsonArg) {

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

	if (jsonArg [PAYLOAD_IDX].success()) {

		int idx = jsonArg [PAYLOAD_IDX];

		// Process message
		if (idx == REMOTE_RELAY_SWITCH_IDX) {

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
		}
	}

	return false;
}