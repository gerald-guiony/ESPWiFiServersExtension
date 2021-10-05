//************************************************************************************************************************
// UdpServer.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once


#include <WiFiUdp.h>					// https://github.com/esp8266/Arduino

#include <Stream/StreamParser.h>
#include <Tools/Delegate.h>

#define DEFAULT_UDP_PORT				555


//------------------------------------------------------------------------------
//
class UdpServer
{
protected:
	WiFiUDP								_wiFiUdp;

public:
	Delegate <> 						notifyPacketReceived;

public:

	void stop							();

	void setup							(uint16_t thePort = DEFAULT_UDP_PORT);
	void loop							();

	virtual StreamParser & getCmdParser () = 0;
};
