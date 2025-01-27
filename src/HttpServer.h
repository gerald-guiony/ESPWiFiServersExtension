//************************************************************************************************************************
// HttpServer.h
// Version 1.0 August, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>

#include "HttpRequestHandler.h"


namespace wifix {

// Important things to remember
// ----------------------------
// * This is fully asynchronous server and as such does not run on the loop thread.
// * You can not use yield or delay or any function that uses them inside the callbacks
// * The server is smart enough to know when to close the connection and free resources
// * You can not send more than one response to a single request

//------------------------------------------------------------------------------
// Singleton
class HttpServer : public AsyncModule <std::vector <HttpRequestHandler *>>
{
	SINGLETON_CLASS(HttpServer)

public:
	Signal <AsyncWebServerRequest *>			notifyRequestReceived;

private:
	void handleNotFound							(AsyncWebServerRequest * request);
	void addRequestHandlers						(std::vector <HttpRequestHandler *> handlers);

public:

	bool isCaptivePortal						(AsyncWebServerRequest * request);

	virtual void setup							(std::vector <HttpRequestHandler *> handlers) override;

	void start									();
	void stop									();
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

}