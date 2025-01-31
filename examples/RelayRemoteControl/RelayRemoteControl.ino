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
void setup() {

	// ------------ Global Init

	EspBoard::init ();

	// ------------- setup

	// If WakeUpFromDeepSleep => No WifiManager & No Ap Mode
	I(wifix::WiFiLinksManagerCustom).setWifiManagerEnabled (!EspBoard::isWakeUpFromDeepSleep());
	I(wifix::WiFiLinksManagerCustom).setup();

#ifdef USING_ETH
#	ifdef ARDUINO_WT32_ETH01
	I(eth::HttpRelayCommandRequestHandler).setup();
#	endif
#endif
	// ------------- Connect signals

	I(wifix::HttpServer).notifyRequestReceived	+= std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#ifdef USING_DOMOTICZ_MQTT
	I(wifix::MqttDomoticzClient).notifyValidMessageReceived += std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#endif
#ifdef USING_AWS_MQTT
	I(wifix::MqttAWSIoTCoreClient).notifyValidMessageReceived += std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#endif

	I(ModuleSequencer).enterDeepSleepWhenWifiOff ();
	I(ModuleSequencer).setup (I(wifix::WiFiLinksManagerCustom).getModules ());
}

//========================================================================================================================
//
//========================================================================================================================
void loop() {
	I(ModuleSequencer).loop ();
}












