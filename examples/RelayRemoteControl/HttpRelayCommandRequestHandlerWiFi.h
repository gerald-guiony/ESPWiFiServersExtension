//************************************************************************************************************************
// HttpRelayCommandRequestHandlerWiFi.h
// Version 1.0 December, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <ESPAsyncWebServer.h>

#include <Common.h>
#include <HttpRequestHandler.h>

#include "HttpRelayCommandRequestHandlerAbstract.h"

namespace wifix {

//------------------------------------------------------------------------------
// Singleton
class HttpRelayCommandRequestHandler : 	public HttpRequestHandler,
										public HttpRelayCommandRequestHandlerAbs <AsyncWebServerRequest, AsyncResponseStream>
{
	SINGLETON_CLASS(HttpRelayCommandRequestHandler)

public:

	virtual void setup (AsyncWebServer & asyncWebServer) override;
};

}










