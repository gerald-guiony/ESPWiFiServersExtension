//************************************************************************************************************************
// UdpClient.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <WiFiUdp.h>					// https://github.com/esp8266/Arduino

#include <Stream/StreamParser.h>


#define DEFAULT_UDP_PORT				555


//------------------------------------------------------------------------------
//
class UdpClient
{
protected:
	WiFiUDP								_wiFiUdp;

	virtual StreamParser & getRespParser () = 0;

public:

	void setup							(uint16_t thePort = DEFAULT_UDP_PORT);

	void sendCmd 						(IPAddress & ip, int port, String & cmd);

	bool isResponseAvailable 			();
	bool parseResponse					(Print & printer);
};
