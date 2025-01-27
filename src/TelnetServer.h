//************************************************************************************************************************
// TelnetServer.h
// Version 2.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <memory>

#ifdef ESP8266
#	include <ESPAsyncTCP.h>							// https://github.com/me-no-dev/ESPAsyncTCP
#elif defined (ESP32)
#	include <AsyncTCP.h>
#endif

#include <Module/AsyncModule.h>
#include <Tools/Signal.h>

using namespace corex;


#define DEFAULT_TELNET_PORT					23


namespace wifix {

//------------------------------------------------------------------------------
//
class TelnetServer : public AsyncModule <uint16_t>
{
protected:

	uint16_t 									_port;
	AsyncServer * 								_server = NULL;

public:
	Signal <AsyncClient *>						notifyClientConnected;
	Signal <AsyncClient *>						notifyClientDisconnected;
	Signal <AsyncClient *, void *, size_t>		notifyClientDataReceived;
	Signal <AsyncClient *, int8_t>				notifyClientError;
	Signal <AsyncClient *, uint32_t>			notifyClientTimeout;

protected:

	virtual void onNewClient					(void * arg, AsyncClient * client);

public:

	TelnetServer								();
	virtual ~TelnetServer 						();

	virtual void setup							(uint16_t thePort = DEFAULT_TELNET_PORT) override;

	virtual void start							();
	virtual void stop							();
};

}










