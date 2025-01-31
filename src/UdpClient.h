//************************************************************************************************************************
// UdpClient.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <WiFiUdp.h>					// https://github.com/esp8266/Arduino

#include <Stream/StreamParser.h>

using namespace corex;


#define DEFAULT_UDP_PORT				555


namespace wifix {

//------------------------------------------------------------------------------
//
class UdpClient : public Module <uint16_t>
{
protected:
	WiFiUDP								_wiFiUdp;

	virtual StreamParser & getRespParser () = 0;

public:

	void sendCmd 						(const IPAddress & ip, int port, const String & cmd);

	bool isResponseAvailable 			();
	bool parseResponse					(Print & printer);

	virtual void setup					(uint16_t thePort = DEFAULT_UDP_PORT) override;
	virtual void loop					() override {}
};

}