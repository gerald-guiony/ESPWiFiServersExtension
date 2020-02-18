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
#define PAYLOAD_IDX						"idx"
#define PAYLOAD_NVALUE					"nvalue"
#define PAYLOAD_SVALUE					"svalue"

#define PAYLOAD_LOG_COMMAND				"command"
#define PAYLOAD_LOG_COMMAND_ADD_MSG		"addlogmessage"
#define PAYLOAD_LOG_MESSAGE				"message"



//------------------------------------------------------------------------------
//
class MqttDomoticzPublisher : public MqttHandler
{
public:
	MqttDomoticzPublisher				() {};

	size_t publishJsonObj 				(const JsonObject& jsonObj, size_t len);	
	virtual size_t publishMessage		(const String & msg) = 0;

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
	void publishStringLine				(const String & msg);
	virtual size_t publishMessage		(const String & msg) override;

	virtual void setup 					(AsyncMqttClient * asyncMqttClient) override;
};

//------------------------------------------------------------------------------
// 
class MqttDomoticzSubscriber : public MqttHandler
{
public:
	MqttDomoticzSubscriber				() {};
	
	void onMqttConnected				(bool sessionPresent);
	void onMqttMessageReceived			(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);

	virtual void parseJsonObj			(const JsonObject& jsonObj) = 0;
	
	virtual void setup 					(AsyncMqttClient * asyncMqttClient) override;

protected:
	AsyncMqttClient * _asyncMqttClient	= nullptr;
};