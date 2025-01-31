//************************************************************************************************************************
// MqttClientAsync.h
// Version 1.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Ticker.h>

#include <Common.h>
#include <Module/AsyncModule.h>

#include "IWiFiLink.h"
#include "MqttHandler.h"


namespace wifix {

//------------------------------------------------------------------------------
//
class MqttClientAsync : public AsyncModule <const char *, int, const std::vector <AsyncMqttHandler *> &>,
						public IWiFiLink
{
protected:

	bool _stopped = false;

	Ticker 			_mqttReconnectTimer;
	AsyncMqttClient _asyncMqttClient;

public:

	Signal <bool> 									notifyConnected;
	Signal <AsyncMqttClientDisconnectReason>		notifyDisconnected;
	Signal <uint16_t, uint8_t> 						notifySubscribed;
	Signal <uint16_t> 								notifyUnsubscribed;
	Signal <char*, char*,
			AsyncMqttClientMessageProperties,
			size_t, size_t, size_t> 				notifyMessageReceived;
	Signal <> 										notifyValidMessageReceived;
	Signal <uint16_t>								notifyPublishSent;

protected:

	void addHandlers								(const std::vector <AsyncMqttHandler *> & handlers);

public:

	MqttClientAsync									();
	virtual ~MqttClientAsync 						();

	virtual void connect				 			() override;
	virtual void disconnect							() override;

	virtual void setup								(const char * ip, int port, const std::vector <AsyncMqttHandler *> & handlers) override;
};

}
