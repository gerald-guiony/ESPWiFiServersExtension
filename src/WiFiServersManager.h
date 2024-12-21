//************************************************************************************************************************
// WiFiServersManager.h
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
#include <Tools/Signal.h>


#define WIFI_RECONNECT_DELAY				2000
#define STA_CONNECTION_DELAY				20000


//------------------------------------------------------------------------------
// WARNING : SINGLETON !!!!
class WiFiServersManager : public Module
{
protected:

	bool _isWifiManagerEnabled									= true;				// By default ...
	bool _accessPointIfNoWifi									= false;
	bool _forceAccessPoint										= false;


public:

	Signal <>													notifyArduinoOTAStart;
	Signal <unsigned int, unsigned int>							notifyArduinoOTAProgress;
	Signal <>													notifyArduinoOTAEnd;

	Signal <const IPAddress &>									notifyConnectedToWiFi;
	Signal <>													notifyDisconnectedFromWiFi;
	Signal <>													notifyClientConnected;
	Signal <>													notifyClientDisconnected;

protected:

	void startWifi 												();
	void startWifiAndServers									();

	bool startWiFiManager										();
	void startOTA												();

	void startAllServers										();
	void stopAllServers											();

	virtual void startCustomServers								() = 0;
	virtual void stopCustomServers								() = 0;

public:

	void setWifiManagerEnabled									(bool enabled, bool accessPointIfNoWifi = false);

	virtual void setup											(bool forceAccessPoint = false);
	virtual void loop											() override;
};
