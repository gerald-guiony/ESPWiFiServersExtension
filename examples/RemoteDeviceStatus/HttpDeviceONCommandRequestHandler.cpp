//************************************************************************************************************************
// HttpDeviceONCommandRequestHandler.cpp
// Version 1.0 December, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#include <Stream.h>
#include <StreamString.h>
#include <ArduinoJson.h>

#include <HttpServer.h>

#include "HttpDeviceONCommandRequestHandler.h"


// Important things to remember
// ----------------------------
// * This is fully asynchronous server and as such does not run on the loop thread.
// * You can not use yield or delay or any function that uses them inside the callbacks
// * The server is smart enough to know when to close the connection and free resources
// * You can not send more than one response to a single request



// HTTP Request Methods
// https://www.w3schools.com/tags/ref_httpmethods.asp



// Use PROGMEM and server.send_P to send data from PROGMEM — in this case it doesn't need to be copied to RAM twice, you avoid allocating Strings and all the associated issues.
static const char PROGMEM PRINT_HELP[] = R"rawliteral(
===========================================================================================================
 *** AVAILABLE IR JSON COMMANDS ON THIS DEVICE ***
===========================================================================================================

-----------------------------------------------------------------------------------------------------------
 Json command format :            /relay/$cmd?json={"$name":"$value", ..}
-----------------------------------------------------------------------------------------------------------

 Supported commands list ($cmd?json={"$name":"$value", ..}) :

 help ............................ Print this help

===========================================================================================================
)rawliteral";



//inline Print & operator <<(Print & printer, JsonObject & jsonObj) { jsonObj.printTo(printer); return printer; }


SINGLETON_IMPL (HttpDeviceONCommandRequestHandler)


//========================================================================================================================
//
//========================================================================================================================
void HttpDeviceONCommandRequestHandler :: handleHelp (AsyncWebServerRequest * request)
{
	Logln (F("=> help"));
	request->send_P(200, F("text/plain"), PRINT_HELP);
}

//========================================================================================================================
//
//========================================================================================================================
void HttpDeviceONCommandRequestHandler :: setup (AsyncWebServer & asyncWebServer)
{
	asyncWebServer.on("/relay/help",	std::bind(&HttpDeviceONCommandRequestHandler::handleHelp,	this, _1));
}












