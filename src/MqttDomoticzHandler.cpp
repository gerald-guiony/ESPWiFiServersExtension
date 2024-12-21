//************************************************************************************************************************
// MqttDomoticzHandler.cpp
// Version 1.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#include <Stream.h>
#include <StreamString.h>

#include "MqttClient.h"

#include "MqttDomoticzHandler.h"



SINGLETON_IMPL (MqttDomoticzLogPublisher)


//========================================================================================================================
//
//========================================================================================================================
size_t MqttDomoticzPublisher :: publishJsonObj (const JsonObject& jsonObj, size_t len) {

	// Print the resulting JSON to a String
	String output;
	jsonObj.printTo(output);

	if (_asyncMqttClient->connected()) {
		uint16_t packetIdPub2 = _asyncMqttClient->publish (DOMOTICS_TOPIC_IN, 2, true, output.c_str());
		// Logln(F("Publishing at QoS 2, packetId: ") << packetIdPub2);
		return len;
	}
	return 0;
}

//========================================================================================================================
//
//========================================================================================================================
size_t MqttDomoticzPublisher :: publishMessage (const uint8_t idx, const String & msg) {

	// publish : "{\"idx\": 21, \"nvalue\": 0, \"svalue\": \"%s\"}"

	DynamicJsonBuffer jsonBuffer;
	JsonObject& jsonObj = jsonBuffer.createObject();

	jsonObj[PAYLOAD_IDX]	= idx;
	jsonObj[PAYLOAD_NVALUE]	= 0;
	jsonObj[PAYLOAD_SVALUE]	= msg;

	return publishJsonObj (jsonObj, msg.length());
}


//========================================================================================================================
//
//========================================================================================================================
size_t MqttDomoticzPublisher :: publishValue (const uint8_t idx, const double svalue) {

	return publishMessage (idx, String (svalue));
}

//========================================================================================================================
//
//========================================================================================================================
size_t MqttDomoticzPublisher :: publishSwitchStatut (const uint8_t idx, const bool isOn) {

	// mosquitto_pub -h 192.168.64.7 -t "domoticz/in" -m '{"command": "switchlight", "idx": 171, "switchcmd": "On" }'

	DynamicJsonBuffer jsonBuffer;
	JsonObject& jsonObj = jsonBuffer.createObject();

	jsonObj[PAYLOAD_COMMAND]		= F("switchlight");
	jsonObj[PAYLOAD_IDX]			= idx;
	jsonObj[PAYLOAD_SWITCH_COMMAND]	= isOn ? F("On") : F("Off");

	return publishJsonObj (jsonObj, isOn ? 2 : 3);
}

//========================================================================================================================
//
//========================================================================================================================
void MqttDomoticzPublisher :: setup (AsyncMqttClient * asyncMqttClient) {

	_asyncMqttClient = asyncMqttClient;
}


/************************************************************************************************************************/

//========================================================================================================================
//
//========================================================================================================================
size_t MqttDomoticzLogPublisher :: publishMessage (const String & logMsg) {

	// publish : "{\"command\" : \"addlogmessage\", \"message\" : \"%s\"}"

	static String str;

	str = logMsg;
	str.replace ('\"', '\'');
	if (str.length() <= 0) return 0;

	DynamicJsonBuffer jsonBuffer;
	JsonObject& jsonObj = jsonBuffer.createObject();

	jsonObj[PAYLOAD_LOG_COMMAND] = PAYLOAD_LOG_COMMAND_ADD_MSG;
	jsonObj[PAYLOAD_LOG_MESSAGE] = str;

	return publishJsonObj (jsonObj, str.length());
}

//========================================================================================================================
//
//========================================================================================================================
void MqttDomoticzLogPublisher :: publishStringLine (const String & logMsg) {

	static String str;

	int end = logMsg.indexOf (_ln);
	if (end < 0) return;

	str = logMsg.substring(0, end);

	size_t msgSizePublished = publishMessage (str);
//	if (msgSizePublished) {
//		sstr.remove (0, msgSizePublished);
//	}
}

//========================================================================================================================
//
//========================================================================================================================
void MqttDomoticzLogPublisher :: setup (AsyncMqttClient * asyncMqttClient) {

	_asyncMqttClient = asyncMqttClient;

//	I(Logger).notifyRequestLineToPrint += std::bind (&MqttDomoticzLogPublisher::publishStringLine, this, _1);
}


/************************************************************************************************************************/


//========================================================================================================================
//
//========================================================================================================================
void MqttDomoticzSubscriber :: onMqttConnected (bool sessionPresent) {

	uint16_t packetIdSub = _asyncMqttClient->subscribe (DOMOTICS_TOPIC_OUT, 2);

	Logln(F("Subscribing at QoS 2, packetId: ") << packetIdSub);
}

//========================================================================================================================
//
//========================================================================================================================
void MqttDomoticzSubscriber :: onMqttMessageReceived (char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {

	// domoticz/out {
		// "Battery" : 255,
		// "RSSI" : 12,
		// "description" : "",
		// "dtype" : "Light/Switch",
		// "id" : "00014051",
		// "idx" : 1,
		// "name" : "Daikin (MQTT)",
		// "nvalue" : 1,
		// "stype" : "Switch",
		// "svalue1" : "0",
		// "switchType" : "On/Off",
		// "unit" : 1
	// }

	// décode le message - decode payload message

	DynamicJsonBuffer jsonBuffer;
	JsonObject& jsonArg = jsonBuffer.parse(payload);

	// Test if parsing succeeds.
	if (jsonArg.success()) {

		// Print the resulting JSON to a String (reformat the payload on a single line)
		String output;
		jsonArg.printTo(output);

		Logln(
			F("Publish received : ") << LN
			<< F(" -topic   =") << topic << LN
			<< F(" -payload =") << output << LN
		);

		if (parseJsonObj (jsonArg))
		{
			I(MqttClient).notifyValidMessageParsed ();
		}
	}
}

//========================================================================================================================
//
//========================================================================================================================
void MqttDomoticzSubscriber :: setup (AsyncMqttClient * asyncMqttClient) {

	_asyncMqttClient = asyncMqttClient;

	I(MqttClient).notifyConnected 		+= std::bind (&MqttDomoticzSubscriber::onMqttConnected, this, _1);
	I(MqttClient).notifyMessageReceived += std::bind (&MqttDomoticzSubscriber::onMqttMessageReceived, this, _1, _2, _3, _4, _5, _6);
}