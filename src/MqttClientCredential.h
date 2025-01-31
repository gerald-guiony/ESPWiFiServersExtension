//************************************************************************************************************************
// MqttClientCredential.h
// Version 1.0 Jan, 2025
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <WiFiClientSecure.h>
#include <PubSubClient.h>

#include <Common.h>

#include "IWiFiLink.h"
#include "MqttHandler.h"


namespace wifix {

//------------------------------------------------------------------------------
//
class MqttClientCredential : public Module <const char *, int, const char *,
											const char *, const char *, const char *,
											const std::vector <MqttHandler *> &>,
							 public IWiFiLink
{
protected:

	bool _stopped = false;
	char * _thingName;

	std::vector <MqttHandler *> _handlers;

#ifdef ESP8266
	BearSSL::WiFiClientSecure _secureClient;
#endif
	PubSubClient _mqttClient;

public:

	Signal <char *, byte *, unsigned int> notifyMessageReceived;
	Signal <> 							  notifyValidMessageReceived;

protected:

	void addHandlers (const std::vector <MqttHandler *> & handlers);

public:

	MqttClientCredential			();
	virtual ~MqttClientCredential	();

	virtual bool isModule 	() const override { return true; }

	virtual void connect  	() override;
	virtual void disconnect	() override;

	virtual void setup (const char * endPoint, int port, const char * thingName,
						const char * rootCA, const char * deviceCert, const char * privateKey,
						const std::vector <MqttHandler *> & handlers) override;

	virtual void loop () override;

};


}