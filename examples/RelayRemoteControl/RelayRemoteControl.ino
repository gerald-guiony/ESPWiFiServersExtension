//************************************************************************************************************************
// RelayRemoteControl.ino
// Version 1.0 December, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#include <Common.h>
#include <HttpServer.h>
#include <MqttDomoticzClient.h>
#include <MqttAWSIoTCoreClient.h>
#include <Switches/BasicRelay.h>

#include "Settings.h"
#include "WiFiLinksManagerCustom.h"

#ifdef ARDUINO_WT32_ETH01
#	include "HttpRelayCommandRequestHandlerEth.h"
#endif

using namespace corex;


std::vector <BasicRelay> relays = { BasicRelay (RELAY_PIN) };



//========================================================================================================================
//
//========================================================================================================================
#ifdef ARDUINO_WT32_ETH01
void startWifiAndEth () {
#else
void startWifi () {
#endif

	// ------------- setup

	// If WakeUpFromDeepSleep => No WifiManager & No Ap Mode
	I(wifix::WiFiLinksManagerCustom).setWifiManagerEnabled (!EspBoard::isWakeUpFromDeepSleep());
	I(wifix::WiFiLinksManagerCustom).setup();

#ifdef ARDUINO_WT32_ETH01
	I(eth::HttpRelayCommandRequestHandler).setup();
#endif

	// ------------- Connect signals

	I(wifix::HttpServer).notifyRequestReceived	+= std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#	ifdef USING_DOMOTICZ_MQTT
	I(wifix::MqttDomoticzClient).notifyValidMessageReceived += std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#	endif
#	ifdef USING_AWS_MQTT
	I(wifix::MqttAWSIoTCoreClient).notifyValidMessageReceived += std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#	endif

	I(ModuleSequencer).enterDeepSleepWhenWifiOff ();
	I(ModuleSequencer).setup (I(wifix::WiFiLinksManagerCustom).getModules ());
}

//========================================================================================================================
//
//========================================================================================================================
void setup() {

	// ------------ Global Init

	EspBoard::init (true);

#ifdef ARDUINO_WT32_ETH01

	pinMode(WIFI_PIN, INPUT);

	// If no Jumper between WIFI_PIN and Ground
	if (analogRead (WIFI_PIN) > 10) {
		startWifiAndEth ();
	}
	else {
		I(eth::HttpRelayCommandRequestHandler).setup();
		I(ModuleSequencer).setup ({});
	}

	EspBoard::enablePowerSavingMode ();

#else

	startWifi ();

#endif
}

//========================================================================================================================
//
//========================================================================================================================
void loop() {
	I(ModuleSequencer).loop ();
}












