//************************************************************************************************************************
// WiFiServersManager.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <IPAddress.h>

#include <ESP8266WiFi.h>							// https://github.com/esp8266/Arduino

#include <PowerMngt/Looper.h>
#include <Tools/Signal.h>


#define WIFI_RECONNECT_DELAY				2000
#define STA_CONNECTION_DELAY				20000


//------------------------------------------------------------------------------
// WARNING : SINGLETON !!!!
class WiFiServersManager : public Looper
{
protected:

	bool _isWifiManagerEnabled									= true;				// By default ...
	bool _accessPointIfNoWifi									= false;
	bool _forceAccessPoint										= false;


public:

	Signal <>													notifyArduinoOTAStart;
	Signal <unsigned int, unsigned int>							notifyArduinoOTAProgress;
	Signal <>													notifyArduinoOTAEnd;

	Signal <const WiFiEventStationModeGotIP&>					notifyWifiConnected;
	Signal <const WiFiEventStationModeDisconnected&>			notifyWifiDisconnected;
	Signal <const WiFiEventSoftAPModeStationConnected&>			notifyStationConnected;
	Signal <const WiFiEventSoftAPModeStationDisconnected&>		notifyStationDisconnected;

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
