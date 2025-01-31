//************************************************************************************************************************
// RemoteDeviceStatus.ino
// Version 1.0 December, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#include <Common.h>
#include <HttpServer.h>
#include <MqttDomoticzClient.h>

#include "Settings.h"
#include "WiFiLinksManagerCustom.h"

using namespace corex;
using namespace wifix;

//========================================================================================================================
//
//========================================================================================================================
void setup() {

	// ------------ Global Init

	EspBoard::init (true);

	// ------------- setup

	 // If WakeUpFromDeepSleep => No WifiManager & No Ap Mode
	I(WiFiLinksManagerCustom).setWifiManagerEnabled (!EspBoard::isWakeUpFromDeepSleep());
	I(WiFiLinksManagerCustom).setup();

	// ------------- Connect signals

	I(HttpServer).notifyRequestReceived	+= std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
	I(MqttDomoticzClient).notifyValidMessageReceived += std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));

	I(ModuleSequencer).setDeepSleepTime (DEVICE_ON_STATE_PUBLISH_PERIOD_ms);

	I(ModuleSequencer).setConditionToEnterDeepSleep ([]() {
		if (EspBoard::isWakeUpFromDeepSleep())
		{
			// The condition to enter deep sleep is true if the ON state has been published
			return I(WiFiLinksManagerCustom).isMqttDeviceOnPublished();
		}
		else // Fresh power-on or other reboot reason...
		{
			// Leave time for debugging or for an update via OTA
			return (millis () > TIME_WITHOUT_DEEPSLEEP_ms);
		}
	});

	I(ModuleSequencer).setup (I(WiFiLinksManagerCustom).getModules ());
}

//========================================================================================================================
//
//========================================================================================================================
void loop() {
	I(ModuleSequencer).loop ();
}












