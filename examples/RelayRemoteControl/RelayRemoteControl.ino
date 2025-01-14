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

	I(CustomWiFiServersManager).setWifiManagerEnabled (!EspBoard::isWakeUpFromDeepSleep());	// If WakeUpFromDeepSleep => No WifiManager & No Ap Mode
	I(CustomWiFiServersManager).setup();

	// ------------- Connect notifiers

	I(HttpServer).notifyRequestReceived	+= std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#ifdef USING_MQTT
	I(MqttClient).notifyValidMessageParsed += std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#endif

	I(ModuleSequencer).enterDeepSleepWhenWifiOff ();										// Deep sleep
	I(ModuleSequencer).setup ({ &I(CustomWiFiServersManager) });
}

//========================================================================================================================
//
//========================================================================================================================
void loop() {
	I(ModuleSequencer).loop ();
}












