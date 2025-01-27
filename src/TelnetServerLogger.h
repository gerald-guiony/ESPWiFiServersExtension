//************************************************************************************************************************
// TelnetServerLogger.h
// Version 2.0 June, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#ifdef ESP8266
#	include <ESPAsyncTCPbuffer.h>							// https://github.com/me-no-dev/ESPAsyncTCP
#elif defined (ESP32)
#	include <AsyncTCPbuffer.h>
#else
#	error Platform not supported
#endif


#include <Common.h>
#include "TelnetServer.h"


#define ASYNC_CLIENT_ACK_TIMEOUT 10000


namespace wifix {

//------------------------------------------------------------------------------
// Singleton
class TelnetServerLogger : public TelnetServer
{
	SINGLETON_CLASS(TelnetServerLogger)

protected:

	void addClient					(AsyncClient * client);
	void deleteClient				(AsyncClient * client);

	void onClientDataReceived		(AsyncClient * client, void *data, size_t len);
	void onClientDisconnected		(AsyncClient * client);

	virtual void onNewClient		(void * arg, AsyncClient * client) override;

public:

	virtual void stop				() override;
	virtual void setup				(uint16_t thePort = DEFAULT_TELNET_PORT) override;
};

}