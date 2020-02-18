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

	Delegate <bool> 								notifyConnected;
	Delegate <AsyncMqttClientDisconnectReason>		notifyDisconnected;
	Delegate <uint16_t, uint8_t> 					notifySubscribed;
	Delegate <uint16_t> 							notifyUnsubscribed;
	Delegate <char*, char*, 
				AsyncMqttClientMessageProperties, 
				size_t, size_t, size_t> 			notifyMessageReceived;
	Delegate <uint16_t>								notifyPublishSent;
	
protected:

	void connectToMqttServer 						();
	
public:

	void addHandlers								(std::vector <MqttHandler *> handlers);
	void stop										();

	void setup										(const char * ip, int port);
	void loop										();
};


