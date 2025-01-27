//************************************************************************************************************************
// WiFiServersManagerCustom.cpp
// Version 1.0 August, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#include <HttpServer.h>
#include <MqttClient.h>

#include "Settings.h"

#include <HttpAdminCommandRequestHandler.h>

#include "MqttDeviceONDomoticzHandler.h"
#include "WiFiServersManagerCustom.h"


namespace wifix {

SINGLETON_IMPL (WiFiServersManagerCustom)


//========================================================================================================================
//
//========================================================================================================================
void WiFiServersManagerCustom :: setupCustomServers ()
{
	// Add the JSON administrative commands request handler
	I(HttpServer).setup ( { &I(HttpAdminCommandRequestHandler) } );

	I(MqttClient).setup (MQTT_SERVER_IP, MQTT_SERVER_PORT, { &I(MqttDomoticzLogPublisher),
															 &I(MqttDeviceONDomoticzPublisher),
															 &I(MqttDeviceONDomoticzSubscriber) });
	I(MqttClient).notifySubscribed += [this] (uint16_t packetId, uint8_t qosk) {
		if (!_isMqttDeviceOnPublished)	{
			I(MqttDeviceONDomoticzPublisher).publishDeviceONState ();
			_isMqttDeviceOnPublished = true;
		}
	};
}

//========================================================================================================================
//
//========================================================================================================================
void WiFiServersManagerCustom :: startCustomServers ()
{
	// Start the Web Servers
	I(HttpServer).start ();
	// Start the MQTT Clients
	I(MqttClient).start ();
}

//========================================================================================================================
//
//========================================================================================================================
void WiFiServersManagerCustom :: stopCustomServers ()
{
	I(HttpServer).stop();
	I(MqttClient).stop();
}

}