//************************************************************************************************************************
// HttpRelayCommandRequestHandler.h
// Version 1.0 December, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <ESPAsyncWebServer.h>

#include <Common.h>
#include <HttpRequestHandler.h>

//------------------------------------------------------------------------------
// Singleton
class HttpRelayCommandRequestHandler : public HttpRequestHandler
{
	SINGLETON_CLASS(HttpRelayCommandRequestHandler)

private:

	void handleHelp			(AsyncWebServerRequest * request);
	void handleOpen			(AsyncWebServerRequest * request);
	void handleClose		(AsyncWebServerRequest * request);

public:

	virtual void setup 		(AsyncWebServer & asyncWebServer) override;
};












