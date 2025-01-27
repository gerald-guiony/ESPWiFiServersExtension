//************************************************************************************************************************
// RelayRemoteControl.ino
// Version 1.0 December, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#include <Common.h>
#include <HttpServer.h>
#include <MqttClient.h>
#include <Switches/BasicRelay.h>

#include "Settings.h"
#include "WiFiServersManagerCustom.h"

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

	EspBoard::init (true);

	// ------------- setup

	// If WakeUpFromDeepSleep => No WifiManager & No Ap Mode
	I(wifix::WiFiServersManagerCustom).setWifiManagerEnabled (!EspBoard::isWakeUpFromDeepSleep());
	I(wifix::WiFiServersManagerCustom).setup();

#ifdef ARDUINO_WT32_ETH01
	I(eth::HttpRelayCommandRequestHandler).setup();
#endif

	// ------------- Connect signals

	I(wifix::HttpServer).notifyRequestReceived	+= std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#ifdef USING_MQTT
	I(wifix::MqttClient).notifyValidMessageParsed += std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#endif

	I(ModuleSequencer).enterDeepSleepWhenWifiOff ();
	I(ModuleSequencer).setup ({ &I(wifix::WiFiServersManagerCustom) });
}

//========================================================================================================================
//
//========================================================================================================================
void loop() {
	I(ModuleSequencer).loop ();
}












