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
#include "CustomWiFiServersManager.h"


std::vector <BasicRelay> relays = { BasicRelay (RELAY_PIN) };


//========================================================================================================================
//
//========================================================================================================================
void setup() {

	// ------------ Global Init

	EspBoard::init (true);

	// ------------- setup

	// If WakeUpFromDeepSleep => No WifiManager & No Ap Mode
	I(CustomWiFiServersManager).setWifiManagerEnabled (!EspBoard::isWakeUpFromDeepSleep());
	I(CustomWiFiServersManager).setup();

	// ------------- Connect signals

	I(HttpServer).notifyRequestReceived	+= std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#ifdef USING_MQTT
	I(MqttClient).notifyValidMessageParsed += std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#endif

	I(ModuleSequencer).enterDeepSleepWhenWifiOff ();
	I(ModuleSequencer).setup ({ &I(CustomWiFiServersManager) });
}

//========================================================================================================================
//
//========================================================================================================================
void loop() {
	I(ModuleSequencer).loop ();
}












