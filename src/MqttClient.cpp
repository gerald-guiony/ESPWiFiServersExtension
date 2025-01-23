//************************************************************************************************************************
// MqttClient.cpp
// Version 1.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#include <Stream.h>
#include <StreamString.h>

#include "MqttClient.h"


#define MQTT_CONNECTION_TIME_MS						20000


static AsyncMqttClient	asyncMqttClient;



SINGLETON_IMPL (MqttClient)


//========================================================================================================================
//
//========================================================================================================================
void MqttClient :: stop () {
	asyncMqttClient.disconnect();
}

//========================================================================================================================
//
//========================================================================================================================
void MqttClient :: connectToMqttServer () {

	Logln(F("Connecting to MQTT..."));
	asyncMqttClient.connect();
}

//========================================================================================================================
//
//========================================================================================================================
void MqttClient :: addHandlers	(std::vector <MqttHandler *> handlers) {

	for (MqttHandler * handler : handlers) {
		handler->setup (&asyncMqttClient);
	}
}

//========================================================================================================================
//
//========================================================================================================================
void MqttClient :: setup (const char * ip, int port) {

	Logln(F("Starting MQTT.."));

	asyncMqttClient.onConnect (
		[this](bool sessionPresent) {
			Logln(F("Connected to MQTT : ") << F("Session present=") << sessionPresent);
			notifyConnected (sessionPresent);
		});
	asyncMqttClient.onDisconnect (
		[this](AsyncMqttClientDisconnectReason reason) {
			Logln(F("Disconnected from MQTT"));
			notifyDisconnected (reason);
		});
	asyncMqttClient.onSubscribe	(
		[this](uint16_t packetId, uint8_t qos) {
			Logln(F("Subscribe acknowledged : ") << F("PacketId=") << packetId << F(", qos=") << qos);
			notifySubscribed (packetId, qos);
		});
	asyncMqttClient.onUnsubscribe (
		[this](uint16_t packetId) {
			Logln(F("Unsubscribe acknowledged : ") << F("PacketId=") << packetId);
			notifyUnsubscribed (packetId);
		});
	asyncMqttClient.onMessage (
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
	asyncMqttClient.onPublish (
		[this](uint16_t packetId) {
			//	Logln(F("Publish acknowledged : ") << F("PacketId=") << packetId);
			notifyPublishSent (packetId);
		});

	asyncMqttClient.setServer (ip, port);
}

//========================================================================================================================
//
//========================================================================================================================
void MqttClient :: loop () {

	static unsigned long _lastConnectionTry = 0;

	if (!asyncMqttClient.connected()) {
		if (WiFiHelper::isWifiAvailable ()) {
			if (millis() - _lastConnectionTry > MQTT_CONNECTION_TIME_MS) {

				connectToMqttServer ();
				_lastConnectionTry = millis();
			}
		}
	}
}