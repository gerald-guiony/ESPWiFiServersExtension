//************************************************************************************************************************
// WiFiLinksManagerCustom.cpp
// Version 1.0 August, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#include <HttpServer.h>
#include <MqttDomoticzClient.h>
#include <HttpAdminCommandRequestHandler.h>

#include "Settings.h"
#include "MqttDeviceONDomoticzHandler.h"

#include "WiFiLinksManagerCustom.h"


namespace wifix {

SINGLETON_IMPL (WiFiLinksManagerCustom)

//========================================================================================================================
//
//========================================================================================================================
void WiFiLinksManagerCustom :: setupCustomLinks ()
{
	// Add the JSON administrative commands request handler
	I(HttpServer).setup ( { &I(HttpAdminCommandRequestHandler) } );

	I(MqttDomoticzClient).setup (MQTT_DOMOTICZ_ENDPOINT, MQTT_DOMOTICZ_PORT, { 	&I(MqttDomoticzLogPublisher),
																	 			&I(MqttDeviceONDomoticzPublisher),
																				&I(MqttDeviceONDomoticzSubscriber) });

	I(MqttDomoticzClient).notifySubscribed += [this] (uint16_t packetId, uint8_t qosk) {
		if (!_isMqttDeviceOnPublished)	{
			I(MqttDeviceONDomoticzPublisher).publishDeviceONState ();
			_isMqttDeviceOnPublished = true;
		}
	};

	addCustomLinks ({&I(HttpServer), &I(MqttDomoticzClient)});
}

}