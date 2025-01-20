//************************************************************************************************************************
// RemoteDeviceStatus.ino
// Version 1.0 December, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#include <Common.h>
#include <HttpServer.h>
#include <MqttClient.h>

#include "Settings.h"
#include "CustomWiFiServersManager.h"


//========================================================================================================================
//
//========================================================================================================================
void setup() {

	// ------------ Global Init

	EspBoard::init ();

	// ------------- setup

	 // If WakeUpFromDeepSleep => No WifiManager & No Ap Mode
	I(CustomWiFiServersManager).setWifiManagerEnabled (!EspBoard::isWakeUpFromDeepSleep());
	I(CustomWiFiServersManager).setup();

	// ------------- Connect notifiers

	I(HttpServer).notifyRequestReceived	+= std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
	I(MqttClient).notifyValidMessageParsed += std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));

	I(ModuleSequencer).setDeepSleepTime(DEVICE_ON_STATE_PUBLISH_PERIOD_ms);

	I(ModuleSequencer).setConditionToEnterDeepSleep ([]() {
		if (EspBoard::isWakeUpFromDeepSleep())
		{
			// The condition is true if the ON state has been published
			return I(CustomWiFiServersManager).isMqttDeviceOnPublished();
		}
		else // Fresh power-on or other reboot reason...
		{
			// Leave time for debugging or for an update via OTA
			return (millis () > TIME_WITHOUT_DEEPSLEEP_ms);
		}
	});

    I(ModuleSequencer).setup ({ &I(CustomWiFiServersManager) });
}

//========================================================================================================================
//
//========================================================================================================================
void loop() {
	I(ModuleSequencer).loop ();
}












