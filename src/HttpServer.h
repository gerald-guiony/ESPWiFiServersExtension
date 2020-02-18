//************************************************************************************************************************
// HttpServer.h
// Version 1.0 August, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <ESPAsyncWebServer.h>

#include <Common.h>

#include "HttpRequestHandler.h"


// Important things to remember
// ----------------------------
// * This is fully asynchronous server and as such does not run on the loop thread.
// * You can not use yield or delay or any function that uses them inside the callbacks
// * The server is smart enough to know when to close the connection and free resources
// * You can not send more than one response to a single request


//------------------------------------------------------------------------------
// Singleton
class HttpServer
{
	SINGLETON_CLASS(HttpServer)

public:
	Delegate <AsyncWebServerRequest *>			notifyRequestReceived;
	
private:
	void handleNotFound							(AsyncWebServerRequest * request);
	
public:
	static AsyncWebServer & getAsyncWebServer	();
	
	bool isCaptivePortal						(AsyncWebServerRequest * request);
	void stop									();

	void addRequestHandlers						(std::vector <HttpRequestHandler *> hanlders);
	
	virtual void setup							();
	virtual void loop							();
};

//------------------------------------------------------------------------------
//
class HttpAsyncWebHandler : public AsyncWebHandler {
public:
	HttpAsyncWebHandler						() {}
	virtual ~HttpAsyncWebHandler			() {}

	bool canHandle							(AsyncWebServerRequest *request) override {
		return true;
	}

	void handleRequest						(AsyncWebServerRequest *request) override {
		I(HttpServer).notifyRequestReceived (request);
	}
};