//************************************************************************************************************************
// TelnetServer.h
// Version 2.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <memory>

#include <ESPAsyncTCP.h>							// https://github.com/me-no-dev/ESPAsyncTCP


#include <Tools/Delegate.h>


#define DEFAULT_TELNET_PORT					23


//------------------------------------------------------------------------------
//
class TelnetServer
{
protected:

	uint16_t 									_port;
	AsyncServer * 								_server = NULL;
	
public:
	Delegate <AsyncClient *>					notifyClientConnected;
	Delegate <AsyncClient *>					notifyClientDisconnected;
	Delegate <AsyncClient *, void *, size_t>	notifyClientDataReceived;
	Delegate <AsyncClient *, int8_t>			notifyClientError;
	Delegate <AsyncClient *, uint32_t>			notifyClientTimeout;
	
protected:

	virtual void onNewClient					(void * arg, AsyncClient * client);

public:		

	TelnetServer								();
	virtual ~TelnetServer 						();

	virtual void stop							();

	virtual void setup							(uint16_t thePort = DEFAULT_TELNET_PORT);
	virtual void loop							();
};












