//************************************************************************************************************************
// CustomWiFiServersManager.cpp
// Version 1.0 August, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#include <HttpServer.h>
#include <MqttClient.h>

#include "Settings.h"

#include <HttpAdminCommandRequestHandler.h>

#include "HttpDeviceONCommandRequestHandler.h"
#include "MqttDeviceONDomoticzHandler.h"

#include "CustomWiFiServersManager.h"


#define MQTT_REFRESH_DEVICE_ON_DELAY_MS						300000		// every 300s == 5 minutes


#ifdef USING_WIFI
#	warning -- USING_WIFI defined --
#	ifdef USING_MQTT
#		warning -- USING_MQTT defined --
#	endif
#endif



SINGLETON_IMPL (CustomWiFiServersManager)


//========================================================================================================================
//
//========================================================================================================================
void CustomWiFiServersManager :: startCustomServers () {

//	if (WiFiHelper::isAccessPointMode()) {

		// Add the administrative commands request handler
		// Only when the device is in Access Point mode (not connected to Wifi)
		// IP address is 192.168.4.1 no password and the IP port is fixed and the same for all esp
		I(HttpServer).addRequestHandlers( { &I(HttpAdminCommandRequestHandler) } );
//	}

	I(HttpServer).addRequestHandlers( { &I(HttpDeviceONCommandRequestHandler) } );

	// Start the Web Server
	I(HttpServer).setup ();


#ifdef USING_MQTT
	if (!WiFiHelper::isAccessPointMode()) {
		// Start the MQTT Client
		I(MqttClient).addHandlers ( { &I(MqttDomoticzLogPublisher), &I(MqttDeviceONDomoticzPublisher), &I(MqttDeviceONDomoticzSubscriber) } );
		I(MqttClient).setup (MQTT_SERVER_IP, MQTT_SERVER_PORT);

	}
#endif
}

//========================================================================================================================
//
//========================================================================================================================
void CustomWiFiServersManager :: stopCustomServers () {

	I(HttpServer).stop();

#ifdef USING_MQTT
	if (!WiFiHelper::isAccessPointMode()) {
		I(MqttClient).stop();
	}
#endif
}

//========================================================================================================================
//
//========================================================================================================================
void CustomWiFiServersManager :: setup (bool forceAccessPoint /*= false*/) {
	WiFiServersManager::setup (forceAccessPoint);
}

//========================================================================================================================
//
//========================================================================================================================
void CustomWiFiServersManager :: loop () {

	static unsigned long lastRefresh = -MQTT_REFRESH_DEVICE_ON_DELAY_MS;  // Force first publish on device startup

	if (WiFiHelper::isWifiAvailable()) {

	    WiFiServersManager::loop ();

		I(HttpServer).loop();

#ifdef USING_MQTT
		if (!WiFiHelper::isAccessPointMode()) {
			I(MqttClient).loop();

			if (millis() - lastRefresh > MQTT_REFRESH_DEVICE_ON_DELAY_MS) {

				I(MqttDeviceONDomoticzPublisher).publishDeviceON ();
				lastRefresh = millis();
			}
		}
#endif
	}
}

