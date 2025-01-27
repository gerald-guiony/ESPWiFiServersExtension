//************************************************************************************************************************
// UdpServer.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once


#include <WiFiUdp.h>					// https://github.com/esp8266/Arduino

#include <Stream/StreamParser.h>
#include <Tools/Signal.h>

using namespace corex;


#define DEFAULT_UDP_PORT				555

namespace wifix {

//------------------------------------------------------------------------------
//
class UdpServer : public Module <uint16_t>
{
protected:
	uint16_t 	_port;
	WiFiUDP		_wiFiUdp;

public:
	Signal <> 	notifyPacketReceived;

public:

	virtual StreamParser & getCmdParser () = 0;

	void start	();
	void stop	();

	void setup	(uint16_t thePort = DEFAULT_UDP_PORT) override;
	void loop	() override;
};

}