//************************************************************************************************************************
// HttpRelayCommandRequestHandlerEth.h
// Version 1.0 Jan, 2025
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#define _ASYNC_WEBSERVER_LOGLEVEL_	2
#include <AsyncWebServer_WT32_ETH01.h>

#include <Ticker.h>

#include <Common.h>
#include <Module/AsyncModule.h>

#include "HttpRelayCommandRequestHandlerAbstract.h"


namespace eth {

//------------------------------------------------------------------------------
// Singleton
class HttpRelayCommandRequestHandler : 	public AsyncModule <>,
										public HttpRelayCommandRequestHandlerAbs <eth::AsyncWebServerRequest, eth::AsyncResponseStream>
{
	SINGLETON_CLASS(HttpRelayCommandRequestHandler)

private:

	Ticker _checkConnectionTimer;

private:

	void handleNotFound	(eth::AsyncWebServerRequest * request);

public:

	virtual void setup () override;
};

}









