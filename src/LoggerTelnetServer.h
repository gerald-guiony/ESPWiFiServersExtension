//************************************************************************************************************************
// LoggerTelnetServer.h
// Version 2.0 June, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#ifdef ESP8266

#	include <ESPAsyncTCPbuffer.h>							// https://github.com/me-no-dev/ESPAsyncTCP

#elif defined (ESP32)

#	include <AsyncTCP.h>

class AsyncTCPbuffer: public Print
{
protected:
	AsyncClient * _asyncClient;

public:

	AsyncTCPbuffer (AsyncClient* c) : _asyncClient (c) {}

	virtual ~AsyncTCPbuffer() {
		if(_asyncClient) {
        	_asyncClient->close();
		}
	}

	size_t write(uint8_t data) override {
    	return write((const char *) &data, 1);
	}

	size_t write(const char* data, size_t len) {
		return _asyncClient->write (data, len);
	}

};

#endif


#include <Common.h>
#include "TelnetServer.h"


#define ASYNC_CLIENT_ACK_TIMEOUT 10000



//------------------------------------------------------------------------------
// Singleton
class LoggerTelnetServer : public TelnetServer
{
	SINGLETON_CLASS(LoggerTelnetServer)

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
