//************************************************************************************************************************
// CustomWiFiServersManager.cpp
// Version 1.0 August, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#include <HttpServer.h>
#include <MqttClient.h>

#include "Settings.h"

#include <HttpAdminCommandRequestHandler.h>

#include "MqttDeviceONDomoticzHandler.h"
#include "CustomWiFiServersManager.h"



SINGLETON_IMPL (CustomWiFiServersManager)


//========================================================================================================================
//
//========================================================================================================================
void CustomWiFiServersManager :: startCustomServers () {

	// Add the administrative commands request handler
	I(HttpServer).addRequestHandlers( { &I(HttpAdminCommandRequestHandler) } );

	// Start the Web Server
	I(HttpServer).setup ();

	if (!WiFiHelper::isAccessPointMode()) {
		// Start the MQTT Client
		I(MqttClient).addHandlers ( { &I(MqttDomoticzLogPublisher), &I(MqttDeviceONDomoticzPublisher), &I(MqttDeviceONDomoticzSubscriber) } );
		I(MqttClient).setup (MQTT_SERVER_IP, MQTT_SERVER_PORT);
	}
}

//========================================================================================================================
//
//========================================================================================================================
void CustomWiFiServersManager :: stopCustomServers () {

	I(HttpServer).stop();

	if (!WiFiHelper::isAccessPointMode()) {
		I(MqttClient).stop();
	}
}

//========================================================================================================================
//
//========================================================================================================================
void CustomWiFiServersManager :: setup (bool forceAccessPoint /*= false*/) {
	WiFiServersManager::setup (forceAccessPoint);

	I(MqttClient).notifySubscribed += [this] (uint16_t packetId, uint8_t qosk) {
		if (!_isMqttDeviceOnPublished)	{
			I(MqttDeviceONDomoticzPublisher).publishDeviceON ();
			_isMqttDeviceOnPublished = true;
		}
	};
}

//========================================================================================================================
//
//========================================================================================================================
void CustomWiFiServersManager :: loop () {

	if (WiFiHelper::isWifiAvailable()) {

	    WiFiServersManager::loop ();
		I(HttpServer).loop();

		if (!WiFiHelper::isAccessPointMode()) {
			I(MqttClient).loop();
		}
	}
}

