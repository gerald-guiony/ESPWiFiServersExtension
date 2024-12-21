//************************************************************************************************************************
// MqttClient.h
// Version 1.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <AsyncMqttClient.h>				// https://github.com/marvinroger/async-mqtt-client

#include <Common.h>

#include "MqttHandler.h"

//------------------------------------------------------------------------------
// Singleton
class MqttClient
{
	SINGLETON_CLASS(MqttClient)

public:

	Signal <bool> 									notifyConnected;
	Signal <AsyncMqttClientDisconnectReason>		notifyDisconnected;
	Signal <uint16_t, uint8_t> 						notifySubscribed;
	Signal <uint16_t> 								notifyUnsubscribed;
	Signal <char*, char*,
				AsyncMqttClientMessageProperties,
				size_t, size_t, size_t> 			notifyMessageReceived;
	Signal <> 										notifyValidMessageParsed;
	Signal <uint16_t>								notifyPublishSent;

protected:

	void connectToMqttServer 						();

public:

	void addHandlers								(std::vector <MqttHandler *> handlers);
	void stop										();

	void setup										(const char * ip, int port);
	void loop										();
};


