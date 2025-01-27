//************************************************************************************************************************
// HttpAdminCommandRequestHandler.h
// Version 1.0 August, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>
#include "HttpRequestHandler.h"


namespace wifix {

//------------------------------------------------------------------------------
// WARNING : SINGLETON !!!!
class HttpAdminCommandRequestHandler : public HttpRequestHandler
{
	SINGLETON_CLASS(HttpAdminCommandRequestHandler)

private:
	void handleHelp				(AsyncWebServerRequest * request);
	void handleInfo				(AsyncWebServerRequest * request);
	void handleReboot			(AsyncWebServerRequest * request);
	void handleListAllFiles		(AsyncWebServerRequest * request);
	void handleDelAllFiles		(AsyncWebServerRequest * request);
	void handleWifi				(AsyncWebServerRequest * request);

public:
	virtual void setup			(AsyncWebServer & asyncWebServer) override;
};

}












