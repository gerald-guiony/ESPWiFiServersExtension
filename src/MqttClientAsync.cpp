//************************************************************************************************************************
// MqttClientAsync.cpp
// Version 1.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#include <Stream.h>
#include <StreamString.h>

#include "MqttClientAsync.h"


#define MQTT_CONNECTION_TIME_MS						2000


namespace wifix {


//========================================================================================================================
//
//========================================================================================================================
MqttClientAsync :: MqttClientAsync () {
}

//========================================================================================================================
//
//========================================================================================================================
MqttClientAsync :: ~MqttClientAsync () {
	disconnect ();
}

//========================================================================================================================
//
//========================================================================================================================
void MqttClientAsync :: connect ()
{
	_stopped = false;
	if (WiFiHelper::isStationModeActive())
	{
		if (!_asyncMqttClient.connected()) {
			Logln(F("Connecting to MQTT..."));
			_asyncMqttClient.connect();
		}
	}
}

//========================================================================================================================
//
//========================================================================================================================
void MqttClientAsync :: disconnect ()
{
	_stopped = true;
	if (WiFiHelper::isStationModeActive())
	{
		_asyncMqttClient.disconnect();
	}
}

//========================================================================================================================
//
//========================================================================================================================
void MqttClientAsync :: addHandlers	(const std::vector <AsyncMqttHandler *> & handlers)
{
	for (AsyncMqttHandler * handler : handlers) {
		handler->setup (&_asyncMqttClient);
	}
}

//========================================================================================================================
//
//========================================================================================================================
void MqttClientAsync :: setup (const char * ip, int port, const std::vector <AsyncMqttHandler *> & handlers)
{
	if (WiFiHelper::isAccessPointMode()) {
		Logln(F("Access point mode => MQTT aborted.."));
		return;
	}

	Logln(F("Setting MQTT.."));

	addHandlers (handlers);

	_asyncMqttClient.onConnect (
		[this](bool sessionPresent) {
			Logln(F("Connected to MQTT : ") << F("Session present=") << sessionPresent);
			_mqttReconnectTimer.detach(); // ensure we don't try to reconnect to MQTT server
			notifyConnected (sessionPresent);
		});
	_asyncMqttClient.onDisconnect (
		[this](AsyncMqttClientDisconnectReason reason) {
			Logln(F("Disconnected from MQTT"));
			if (!_stopped) _mqttReconnectTimer.attach_ms (MQTT_CONNECTION_TIME_MS, std::bind(&MqttClientAsync::connect, this));
			notifyDisconnected (reason);
		});
	_asyncMqttClient.onSubscribe	(
		[this](uint16_t packetId, uint8_t qos) {
			Logln(F("Subscribe acknowledged : ") << F("PacketId=") << packetId << F(", qos=") << qos);
			notifySubscribed (packetId, qos);
		});
	_asyncMqttClient.onUnsubscribe (
		[this](uint16_t packetId) {
			Logln(F("Unsubscribe acknowledged : ") << F("PacketId=") << packetId);
			notifyUnsubscribed (packetId);
		});
	_asyncMqttClient.onMessage (
		[this](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
			//	Logln(
			//		F("Publish received : ") << LN
			//		<< F(" -topic   =") << topic << LN
			//		<< F(" -payload =") << payload << LN
			//		<< F(" -qos	 =") 	<< properties.qos << F(", dup=") << properties.dup << F(", retain=") << properties.retain << LN
			//		<< F(" -len	 =") 	<< len << LN
			//		<< F(" -index   =") << index << LN
			//		<< F(" -total   =") << total << LN
			//	);
			notifyMessageReceived (topic, payload, properties, len, index, total);
		});
	_asyncMqttClient.onPublish (
		[this](uint16_t packetId) {
			//	Logln(F("Publish acknowledged : ") << F("PacketId=") << packetId);
			notifyPublishSent (packetId);
		});

	_asyncMqttClient.setServer (ip, port);
}

}