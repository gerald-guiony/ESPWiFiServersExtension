//************************************************************************************************************************
// MqttClient.h
// Version 1.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>
#include <Module/AsyncModule.h>

#include "MqttHandler.h"


namespace wifix {

//------------------------------------------------------------------------------
// Singleton
//
// WARNING : Not a true AsyncModule because we need to periodically check the
// 			 connection to the server
//
class MqttClient : public AsyncModule <const char *, int, std::vector <MqttHandler *>>
{
	SINGLETON_CLASS(MqttClient)

private:

	bool _stopped = false;

public:

	Signal <bool> 									notifyConnected;
	Signal <AsyncMqttClientDisconnectReason>		notifyDisconnected;
	Signal <uint16_t, uint8_t> 						notifySubscribed;
	Signal <uint16_t> 								notifyUnsubscribed;
	Signal <char*, char*,
			AsyncMqttClientMessageProperties,
			size_t, size_t, size_t> 				notifyMessageReceived;
	Signal <> 										notifyValidMessageParsed;
	Signal <uint16_t>								notifyPublishSent;

protected:

	void addHandlers								(std::vector <MqttHandler *> handlers);

public:

	void setup										(const char * ip, int port, std::vector <MqttHandler *> handlers) override;

	void start				 						();
	void stop										();
};

}
