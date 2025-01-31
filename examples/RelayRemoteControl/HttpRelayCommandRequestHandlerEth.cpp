//************************************************************************************************************************
// HttpRelayCommandRequestHandlerEth.cpp
// Version 1.0 Jan, 2025
// Author Gerald Guiony
//************************************************************************************************************************

#ifdef ARDUINO_WT32_ETH01

#include <WiFi.h>

#include "HttpRelayCommandRequestHandlerEth.h"


namespace eth {

// Select the IP address according to your local network
IPAddress myIP(192, 168, 1, 200);
IPAddress myGW(192, 168, 1, 254);
IPAddress mySN(255, 255, 255, 0);

// Google DNS Server IP
IPAddress myDNS(8, 8, 8, 8);


static AsyncWebServer asyncWebServer (80);


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
void HttpRelayCommandRequestHandler :: handleNotFound (AsyncWebServerRequest *request)
{
	Logln(F("handleNotFound"));

	StreamString sstr;
	sstr <<
	F("File Not Found") << LN << LN <<
	F("URI: ") << request->url() << LN <<
	F("Method: ") << ((request->method() == HTTP_GET) ? "GET":"POST") << LN <<
	F("Arguments: ") << request->args() << LN;

	for (uint8_t i=0; i<request->args(); i++) {
		sstr << F(" ") << request->argName(i) << F(": ") << request->arg(i) << LN;
	}

	// https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
	eth::AsyncWebServerResponse * response = request->beginResponse(404, "text/plain", sstr);
	response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
	response->addHeader("Pragma", "no-cache");
	response->addHeader("Expires", "-1");
	request->send (response);

	Logln(sstr);
}

//========================================================================================================================
//
//========================================================================================================================
void HttpRelayCommandRequestHandler :: setup ()
{
	WiFi.onEvent(
	[this](WiFiEvent_t event, WiFiEventInfo_t info) {

		asyncWebServer.on("/relay/help",	std::bind(&HttpRelayCommandRequestHandler::handleHelp,	this, _1));
		asyncWebServer.on("/relay/open",	std::bind(&HttpRelayCommandRequestHandler::handleOpen,	this, _1));
		asyncWebServer.on("/relay/close",	std::bind(&HttpRelayCommandRequestHandler::handleClose,	this, _1));
		// Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
		asyncWebServer.onNotFound (std::bind(&HttpRelayCommandRequestHandler::handleNotFound, this, _1));

		asyncWebServer.begin();

		Logln(F("ETH Async Web Server is up !"));
	},
	WiFiEvent_t::ARDUINO_EVENT_ETH_GOT_IP);

	// To be called before ETH.begin()
	WT32_ETH01_onEvent();
	ETH.begin();

	// Static IP, leave without this line to get IP via DHCP
	ETH.config(myIP, myGW, mySN, myDNS);

	// Trop hardcore...
	// WT32_ETH01_waitForConnect();
}

}


#endif



