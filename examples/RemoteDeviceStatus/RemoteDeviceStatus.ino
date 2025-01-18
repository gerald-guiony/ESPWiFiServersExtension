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

	I(ModuleSequencer).setDeepSleepTime(TIME_FOR_REFRESH_DEVICE_ON_STATUS_MS);

	I(ModuleSequencer).setConditionToEnterDeepSleep ([]() {
		// Deep sleep cycle period: TIME_FOR_REFRESH_DEVICE_ON_STATUS_MS
		if (EspBoard::isWakeUpFromDeepSleep())
		{
			return I(CustomWiFiServersManager).isMqttDeviceOnPublished();
		}
		// Fresh power-on or other reboot reason...
		else
		{
			return (millis () > TIME_AFTER_POWER_ON_FOR_OTA_UPDATE_MS);
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












