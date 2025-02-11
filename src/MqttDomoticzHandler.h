//************************************************************************************************************************
// MqttDomoticzHandler.h
// Version 1.0 February, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Print.h>
#include <ArduinoJson.h>

#include <Common.h>

#include "MqttHandler.h"

//------------------------------------------------------------------------------
// Domoticz
//

#define DOMOTICS_TOPIC_IN				"domoticz/in"		// Topic MQTT pour les souscriptions a Domoticz - Topic MQTT for Pan servor
#define DOMOTICS_TOPIC_OUT				"domoticz/out"		// Topic MQTT pour les publications vers Domoticz - Topic MQTT for Pan servor

#define PAYLOAD_NAME					"name"
#define PAYLOAD_COMMAND					"command"
#define PAYLOAD_SWITCH_COMMAND			"switchcmd"
#define PAYLOAD_IDX						"idx"
#define PAYLOAD_NVALUE					"nvalue"
#define PAYLOAD_SVALUE					"svalue"

#define PAYLOAD_LOG_COMMAND				"command"
#define PAYLOAD_LOG_COMMAND_ADD_MSG		"addlogmessage"
#define PAYLOAD_LOG_MESSAGE				"message"


namespace wifix {

//------------------------------------------------------------------------------
//
class MqttDomoticzPublisher : public AsyncMqttHandler
{
public:
	MqttDomoticzPublisher				() {};

	size_t publishJsonObj 				(const JsonObject& jsonObj, size_t len);

	size_t publishMessage 				(const uint8_t idx, const String & msg);
	size_t publishValue 				(const uint8_t idx, const double value);
	size_t publishSwitchStatus 			(const uint8_t idx, const bool isOn);

	virtual void setup 					(AsyncMqttClient * asyncMqttClient) override;

protected:
	AsyncMqttClient * _asyncMqttClient	= nullptr;
};

//------------------------------------------------------------------------------
// Singleton
class MqttDomoticzLogPublisher : public MqttDomoticzPublisher
{
	SINGLETON_CLASS(MqttDomoticzLogPublisher)

public:
	virtual size_t publishMessage		(const String & msg);
	void publishStringLine				(const String & msg);


	virtual void setup 					(AsyncMqttClient * asyncMqttClient) override;
};

//------------------------------------------------------------------------------
//
class MqttDomoticzSubscriber : public AsyncMqttHandler
{
public:
	MqttDomoticzSubscriber				() {}

	virtual void setup 					(AsyncMqttClient * asyncMqttClient) override;

protected:

	void onMqttConnected				(bool sessionPresent);
	void onTopicReceived				(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);

	virtual bool parseTopicReceived		(const JsonObject& jsonObj) = 0;

protected:
	AsyncMqttClient * _asyncMqttClient	= nullptr;
};

//------------------------------------------------------------------------------
//
class MqttDomoticzSubscriberIdx : public MqttDomoticzSubscriber
{
public:
	MqttDomoticzSubscriberIdx			() {}

	virtual bool onTopicIdxReceived		(const JsonObject& jsonObj) = 0;

protected:
	virtual bool parseTopicReceived		(const JsonObject& jsonObj) override;

protected:
	int _idx;
};


}