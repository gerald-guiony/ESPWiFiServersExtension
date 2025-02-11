//************************************************************************************************************************
// MqttClientCredential.cpp
// Version 1.0 Jan, 2025
// Author Gerald Guiony
//************************************************************************************************************************


#include "MqttClientCredential.h"


namespace wifix {

//========================================================================================================================
//
//========================================================================================================================
MqttClientCredential :: MqttClientCredential ()
{
}

//========================================================================================================================
//
//========================================================================================================================
MqttClientCredential :: ~MqttClientCredential ()
{
	disconnect ();
}

//========================================================================================================================
//
//========================================================================================================================
void MqttClientCredential :: connect ()
{
	_stopped = false;
	if (WiFiHelper::isStationModeActive()) {

		if (!_mqttClient.connected()) {

			Logln(F("Connecting MQTT Credential"));

			while (!_mqttClient.connect(_thingName)) {
				Log(F("."));
				delay(100);
			}
			Logln(F(""));

			if (!_mqttClient.connected()) {
				Logln(F("MQTT Credential Timeout!"));
				return;
			}

			// Subscribe to a topic, the incoming messages are processed by messageHandler() function
//			_mqttClient.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

			Logln(F("MQTT Credential Connected!"));
		}
	}
}

//========================================================================================================================
//
//========================================================================================================================
void MqttClientCredential :: disconnect ()
{
	_stopped = true;
	if (WiFiHelper::isStationModeActive())
	{
		if (_mqttClient.connected())
		{
			_mqttClient.disconnect();
		}
	}
}

//========================================================================================================================
//
//========================================================================================================================
void MqttClientCredential :: addHandlers (const std::vector <MqttHandler *> & handlers)
{
	for (MqttHandler * handler : handlers) {
		_handlers.push_back (handler);
		handler->setup (&_mqttClient);
	}
}

//========================================================================================================================
//
//========================================================================================================================
void MqttClientCredential :: setup (const char * endPoint, int port, const char * thingName,
									const char * certCa, const char * certCrt, const char * privateKey,
									const std::vector <MqttHandler *> & handlers)
{
	if (WiFiHelper::isAccessPointMode()) {
		Logln(F("Access point mode => MQTT aborted.."));
		return;
	}

	Logln(F("Setting MQTT.."));

	addHandlers (handlers);

	_thingName = (char *) thingName;

#ifdef ESP8266
	// Configure the AWS IoT device credentials
	static BearSSL::X509List cert (certCa);
	static BearSSL::X509List client_crt (certCrt);
	static BearSSL::PrivateKey key (privateKey);

	_secureClient.setTrustAnchors(&cert);				// Charge les autorités de certification.
	_secureClient.setClientRSACert(&client_crt, &key);	// Associe le certificat client avec sa clé privée.

	_mqttClient.setClient (_secureClient);

  	// Connect to the MQTT broker on the AWS endpoint we defined earlier
	_mqttClient.setServer (endPoint, port);

  	// Create a handler for incoming messages
	_mqttClient.setCallback (
		[this](char *topic, byte *payload, unsigned int length) {
			notifyMessageReceived (topic, payload, length);
		}
	);
#endif
}

//========================================================================================================================
//
//========================================================================================================================
void MqttClientCredential :: loop()
{
	if (!_stopped)
	{
		if (!_mqttClient.connected())
		{
			connect ();
		}
		else
		{
			_mqttClient.loop();

			for (MqttHandler * handler : _handlers) {
				handler->loop ();
			}
		}
	}
}

}