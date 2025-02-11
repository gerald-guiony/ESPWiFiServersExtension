//************************************************************************************************************************
// RemoteDeviceStatus.ino
// Version 1.0 December, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#include <Common.h>
#include <HttpServer.h>
#include <HttpAdminCommandRequestHandler.h>
#include <MqttDomoticzClient.h>
#include <WiFiConnectionManager.h>

#include "Settings.h"
#include "MqttDeviceONDomoticzHandler.h"

using namespace corex;
using namespace wifix;


static bool isMqttDeviceOnPublished = false;

//========================================================================================================================
//
//========================================================================================================================
void setup() {

	// ------------ Global Init

	EspBoard::init (true);
	
	// ------------- Connect signals

	I(MqttDomoticzClient).notifySubscribed += [] (uint16_t packetId, uint8_t qosk) {
		if (!isMqttDeviceOnPublished)	{
			I(MqttDeviceONDomoticzPublisher).publishDeviceONState ();
			isMqttDeviceOnPublished = true;
		}
	};

	I(HttpServer).notifyRequestReceived	+= std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
	I(MqttDomoticzClient).notifyValidTopicReceived += std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
	
	// ------------- Module sequencer

	I(ModuleSequencer).setDeepSleepTime (DEVICE_ON_STATE_PUBLISH_PERIOD_ms);
	I(ModuleSequencer).setConditionToEnterDeepSleep ([]() {
		if (EspBoard::isWakeUpFromDeepSleep())	{
			// The condition to enter deep sleep is true if the ON state has been published
			return isMqttDeviceOnPublished;
		}
		else { // Fresh power-on or other reboot reason...
			// Leave time for debugging or for an update via OTA
			return (millis () > TIME_WITHOUT_DEEPSLEEP_ms);
		}
	});

	// ------------- Setup

	 // If WakeUpFromDeepSleep => No WifiManager & No Ap Mode
	I(WiFiConnectionManager).setWifiManagerEnabled (!EspBoard::isWakeUpFromDeepSleep());
	I(WiFiConnectionManager).notifySetupWifiConnections += []() {

		I(HttpServer).setup ({ 	&I(HttpAdminCommandRequestHandler) });
		I(MqttDomoticzClient).setup (MQTT_DOMOTICZ_ENDPOINT, MQTT_DOMOTICZ_PORT, {	&I(MqttDomoticzLogPublisher),
																					&I(MqttDeviceONDomoticzPublisher),
																					&I(MqttDeviceONDomoticzSubscriber) });
	};
	I(WiFiConnectionManager).setup ({	&I(HttpServer),
								 		&I(MqttDomoticzClient) });

	I(ModuleSequencer).setup (I(WiFiConnectionManager).getModules ());
}

//========================================================================================================================
//
//========================================================================================================================
void loop() {
	I(ModuleSequencer).loop ();
}












