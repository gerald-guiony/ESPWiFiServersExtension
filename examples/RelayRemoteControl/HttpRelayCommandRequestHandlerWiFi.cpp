//************************************************************************************************************************
// HttpRelayCommandRequestHandlerWiFi.cpp
// Version 1.0 December, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#include "HttpRelayCommandRequestHandlerWiFi.h"


namespace wifix {


// Important things to remember
// ----------------------------
// * This is fully asynchronous server and as such does not run on the loop thread.
// * You can not use yield or delay or any function that uses them inside the callbacks
// * The server is smart enough to know when to close the connection and free resources
// * You can not send more than one response to a single request

SINGLETON_IMPL (HttpRelayCommandRequestHandler)


//========================================================================================================================
//
//========================================================================================================================
void HttpRelayCommandRequestHandler :: setup (AsyncWebServer & asyncWebServer)
{
	asyncWebServer.on("/relay/help",	std::bind(&HttpRelayCommandRequestHandler::handleHelp,	this, _1));
	asyncWebServer.on("/relay/open",	std::bind(&HttpRelayCommandRequestHandler::handleOpen,	this, _1));
	asyncWebServer.on("/relay/close",	std::bind(&HttpRelayCommandRequestHandler::handleClose,	this, _1));
}

}









