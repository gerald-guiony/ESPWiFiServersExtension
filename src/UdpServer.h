//************************************************************************************************************************
// UdpServer.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once


#include <WiFiUdp.h>					// https://github.com/esp8266/Arduino

#include <Stream/StreamParser.h>
#include <Tools/Signal.h>

#include "IWiFiConnection.h"

using namespace corex;


#define DEFAULT_UDP_PORT				555

namespace wifix {

//------------------------------------------------------------------------------
//
class UdpServer : public Module <uint16_t>, public IWiFiConnection
{
protected:
	uint16_t 	_port;
	WiFiUDP		_wiFiUdp;

public:
	Signal <> 	notifyPacketReceived;

public:

	virtual StreamParser & getCmdParser () = 0;

	virtual bool isModule 	() const override { return true; }

	virtual void connect	() override;
	virtual void disconnect	() override;

	void setup	(uint16_t thePort = DEFAULT_UDP_PORT) override;
	void loop	() override;
};

}