//************************************************************************************************************************
// WiFiConnectionManager.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <IPAddress.h>

#ifdef ESP8266
#	include <ESP8266WiFi.h>							// https://github.com/esp8266/Arduino
#elif defined(ESP32)
#	include <WiFi.h>
#endif

#include <Module/Module.h>
#include <Module/AsyncModule.h>

#include <Tools/Signal.h>

using namespace corex;

#define WIFI_RECONNECT_TIME				2000
#define STA_CONNECTION_TIME				20000

namespace wifix {

//------------------------------------------------------------------------------
//
class WiFiConnectionManager : public Module <const std::vector <IWiFiConnection *> &, bool>
{
	SINGLETON_CLASS(WiFiConnectionManager)

protected:

	bool _isWifiManagerEnabled					= true;				// By default ...
	bool _accessPointIfNoWifi					= false;
	bool _forceAccessPoint						= false;

	std::vector <IWiFiConnection *> 			_wifiConnections;

public:

	Signal <>									notifySetupWifiConnections;

	Signal <>									notifyArduinoOTAStart;
	Signal <unsigned int, unsigned int>			notifyArduinoOTAProgress;
	Signal <>									notifyArduinoOTAEnd;

	Signal <const IPAddress &>					notifyConnectedToWiFi;
	Signal <>									notifyDisconnectedFromWiFi;
	Signal <>									notifyClientConnected;
	Signal <>									notifyClientDisconnected;

protected:

	void addWifiConnections						(const std::vector <IWiFiConnection *> & wifiConnections);

	void setupWifi								();
	void setupOTA								();

	bool startWiFiManager						();
	void startWifi 								();
	void startOTA 								();

	void setupWifiConnections 					();
	void connectWifiConnections					();
	void disconnectWifiConnections				();

public:

	void setWifiManagerEnabled					(bool enabled, bool accessPointIfNoWifi = false);

	std::list <IModule *> getModules 			();

	virtual void setup							(const std::vector <IWiFiConnection *> & wifiConnections, bool forceAccessPoint = false) override;
	virtual void loop							() override;
};

}