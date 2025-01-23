//************************************************************************************************************************
// HttpRelayCommandRequestHandler.cpp
// Version 1.0 December, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#include <Stream.h>
#include <StreamString.h>
#include <ArduinoJson.h>

#include <HttpServer.h>
#include <Switches/BasicRelay.h>

#include "HttpRelayCommandRequestHandler.h"


extern std::vector <BasicRelay> relays;


// Important things to remember
// ----------------------------
// * This is fully asynchronous server and as such does not run on the loop thread.
// * You can not use yield or delay or any function that uses them inside the callbacks
// * The server is smart enough to know when to close the connection and free resources
// * You can not send more than one response to a single request



// HTTP Request Methods
// https://www.w3schools.com/tags/ref_httpmethods.asp



// Use PROGMEM and server.send_P to send data from PROGMEM — in this case it doesn't need to be copied
// to RAM twice, you avoid allocating Strings and all the associated issues.
static const char PROGMEM PRINT_HELP[] = R"rawliteral(
===========================================================================================================
 *** AVAILABLE IR JSON COMMANDS ON THIS DEVICE ***
===========================================================================================================

-----------------------------------------------------------------------------------------------------------
 Json command format :            /relay/$cmd?json={"$name":"$value", ..}
-----------------------------------------------------------------------------------------------------------

 Supported commands list ($cmd?json={"$name":"$value", ..}) :

 help ............................ Print this help
 open : {"id": $1} ............... Open the relay with Id between 0 to 255
 close : {"id": $1} .............. Close the relay with Id between 0 to 255

===========================================================================================================
)rawliteral";



//inline Print & operator <<(Print & printer, JsonObject & jsonObj) { jsonObj.printTo(printer); return printer; }


SINGLETON_IMPL (HttpRelayCommandRequestHandler)


//========================================================================================================================
//
//========================================================================================================================
void HttpRelayCommandRequestHandler :: handleHelp (AsyncWebServerRequest * request)
{
	Logln (F("=> help"));
	request->send_P(200, F("text/plain"), PRINT_HELP);
}

//========================================================================================================================
//
//========================================================================================================================
void HttpRelayCommandRequestHandler :: handleOpen (AsyncWebServerRequest * request)
{
	Logln (F("=> open relay"));

	if (!request->hasArg("json")) {
		request->send(400, F("text/plain"), F("400: json argument not found"));				// The request is invalid, so send HTTP status 400
		return;
	}

	// StaticJsonBuffer allocates memory on the stack, it can be
	// replaced by DynamicJsonBuffer which allocates in the heap.
	DynamicJsonBuffer jsonBuffer;
	JsonObject& jsonArg = jsonBuffer.parse(request->arg("json"));

	// Test if parsing succeeds.
	if (!jsonArg.success()) {
		request->send(400, F("text/plain"), F("400: Invalid json argument"));
		return;
	}

	if (!jsonArg ["id"].success()) {
		request->send(400, F("text/plain"), F("400: Invalid json field"));
		return;
	}

	int id = jsonArg ["id"];
	if ((id < 0) || (relays.size() <= id)) {
		request->send(400, F("text/plain"), F("400: Invalid id argument"));
		return;
	}

	// Visual indicator that signal sent
	EspBoard::blinks (2);

	relays[id].open();

	// This way of sending Json is great for when the result is below 4KB
	AsyncResponseStream * response = request->beginResponseStream(F("application/json"));
	JsonObject & jsonRsp = jsonBuffer.createObject();

	jsonRsp["command"] = "open";
	jsonRsp["status"] = true;

	jsonRsp.prettyPrintTo(*response);
	request->send(response);
}

//========================================================================================================================
//
//========================================================================================================================
void HttpRelayCommandRequestHandler :: handleClose (AsyncWebServerRequest * request)
{
	Logln (F("=> close relay"));

	if (!request->hasArg("json")) {
		request->send(400, F("text/plain"), F("400: json argument not found"));				// The request is invalid, so send HTTP status 400
		return;
	}

	// StaticJsonBuffer allocates memory on the stack, it can be
	// replaced by DynamicJsonBuffer which allocates in the heap.
	DynamicJsonBuffer jsonBuffer;
	JsonObject& jsonArg = jsonBuffer.parse(request->arg("json"));

	// Test if parsing succeeds.
	if (!jsonArg.success()) {
		request->send(400, F("text/plain"), F("400: Invalid json argument"));
		return;
	}

	if (!jsonArg ["id"].success()) {
		request->send(400, F("text/plain"), F("400: Invalid json field"));
		return;
	}

	int id = jsonArg ["id"];
	if ((id < 0) || (relays.size() <= id)) {
		request->send(400, F("text/plain"), F("400: Invalid id argument"));
		return;
	}

	// Visual indicator that signal sent
	EspBoard::blinks (5);

	relays[id].close();

	// This way of sending Json is great for when the result is below 4KB
	AsyncResponseStream * response = request->beginResponseStream(F("application/json"));
	JsonObject & jsonRsp = jsonBuffer.createObject();

	jsonRsp["command"] = "close";
	jsonRsp["status"] = true;

	jsonRsp.prettyPrintTo(*response);
	request->send(response);
}

//========================================================================================================================
//
//========================================================================================================================
void HttpRelayCommandRequestHandler :: setup (AsyncWebServer & asyncWebServer)
{
	asyncWebServer.on("/relay/help",	std::bind(&HttpRelayCommandRequestHandler::handleHelp,	this, _1));
	asyncWebServer.on("/relay/open",	std::bind(&HttpRelayCommandRequestHandler::handleOpen,	this, _1));
	asyncWebServer.on("/relay/close",	std::bind(&HttpRelayCommandRequestHandler::handleClose,	this, _1));
}












