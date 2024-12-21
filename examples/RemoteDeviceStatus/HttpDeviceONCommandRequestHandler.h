//************************************************************************************************************************
// HttpDeviceONCommandRequestHandler.h
// Version 1.0 December, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <ESPAsyncWebServer.h>

#include <Common.h>
#include <HttpRequestHandler.h>

//------------------------------------------------------------------------------
// WARNING : SINGLETON !!!!
class HttpDeviceONCommandRequestHandler : public HttpRequestHandler
{
	SINGLETON_CLASS(HttpDeviceONCommandRequestHandler)

private:

	void handleHelp									(AsyncWebServerRequest * request);

public:

	virtual void setup 								(AsyncWebServer & asyncWebServer) override;
};












