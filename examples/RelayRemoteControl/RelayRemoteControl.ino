//************************************************************************************************************************
// RelayRemoteControl.ino
// Version 1.0 December, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#include <Common.h>
#include <Switches/BasicRelay.h>
#include <Switches/Jumper.h>

#include <HttpServer.h>
#include <HttpAdminCommandRequestHandler.h>
#include <MqttDomoticzClient.h>
#include <MqttAWSIoTCoreClient.h>
#include <WiFiConnectionManager.h>

#include "secrets.h"
#include "Settings.h"
#include "HttpRelayCommandRequestHandlerWiFi.h"
#ifdef ARDUINO_WT32_ETH01
#	include "HttpRelayCommandRequestHandlerEth.h"
#endif
#include "MqttRelayDomoticzHandler.h"


#ifdef USING_AWS_MQTT
#	warning -- USING_AWS_MQTT defined --
#endif

using namespace corex;


BasicRelay basicRelay (RELAY_PIN);

//========================================================================================================================
//
//========================================================================================================================
#ifdef ARDUINO_WT32_ETH01
void startWifiAndEth () {
#else
void startWifi () {
#endif

	// ------------- Connect signals

	I(wifix::HttpServer).notifyRequestReceived	+= std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
	I(wifix::MqttDomoticzClient).notifyValidTopicReceived += std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#ifdef USING_AWS_MQTT
	I(wifix::MqttAWSIoTCoreClient).notifyValidTopicReceived += std::bind (&ModuleSequencer::requestWakeUp, &I(ModuleSequencer));
#endif

	// ------------- Module sequencer

	I(ModuleSequencer).enterDeepSleepWhenWifiOff ();

	// ------------- Setup

	// If WakeUpFromDeepSleep => No WifiManager & No Ap Mode
	I(wifix::WiFiConnectionManager).setWifiManagerEnabled (!EspBoard::isWakeUpFromDeepSleep());
	I(wifix::WiFiConnectionManager).notifySetupWifiConnections += []() {

		I(wifix::HttpServer).setup ({ 	&I(wifix::HttpAdminCommandRequestHandler),
										&I(wifix::HttpRelayCommandRequestHandler) });

		I(wifix::MqttDomoticzClient).setup (MQTT_DOMOTICZ_ENDPOINT, MQTT_DOMOTICZ_PORT, {	&I(wifix::MqttDomoticzLogPublisher),
																							&I(wifix::MqttRelayDomoticzPublisher),
																							&I(wifix::MqttRelayDomoticzSubscriber) });

#ifdef USING_AWS_MQTT
		// Setup AWS Iot Core MQTT client
		I(wifix::MqttAWSIoTCoreClient).setup (	AWS_IOT_ENDPOINT, AWS_IOT_PORT, THINGNAME,
												AWS_CERT_CA, AWS_CERT_CRT, AWS_CERT_PRIVATE, {});
#endif
	};
	I(wifix::WiFiConnectionManager).setup ({	&I(wifix::HttpServer),
								 				&I(wifix::MqttDomoticzClient),
#ifdef USING_AWS_MQTT
												&I(wifix::MqttAWSIoTCoreClient)
#endif
										  });

#ifdef ARDUINO_WT32_ETH01
	I(eth::HttpRelayCommandRequestHandler).setup();
#endif

	I(ModuleSequencer).setup (I(wifix::WiFiConnectionManager).getModules ());
}

//========================================================================================================================
//
//========================================================================================================================
void setup() {

	// ------------ Global Init

	EspBoard::init ();

#ifdef ARDUINO_WT32_ETH01

	Jumper jumper (JUMPER_PIN);

	// If Jumper between WIFI_PIN and Ground
	if (jumper.isConnectedToGround()) {
		I(eth::HttpRelayCommandRequestHandler).setup();
		I(ModuleSequencer).setup ({});
	}
	else {
		startWifiAndEth ();
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












