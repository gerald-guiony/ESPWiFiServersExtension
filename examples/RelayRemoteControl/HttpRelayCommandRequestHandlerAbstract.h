//************************************************************************************************************************
// HttpRelayCommandRequestHandlerAbstract.h
// Version 1.0 December, 2025
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Stream.h>
#include <StreamString.h>
#include <ArduinoJson.h>

#include <Common.h>
#include <Switches/BasicRelay.h>

using namespace corex;


extern std::vector <BasicRelay> relays;


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


//------------------------------------------------------------------------------
//
template <class TAsyncRequest, class TAsyncResponseStream>
class HttpRelayCommandRequestHandlerAbs
{

protected:

	//========================================================================================================================
	//
	//========================================================================================================================
	void handleHelp (TAsyncRequest * request)
	{
		Logln (F("=> help"));
		request->send_P(200, F("text/plain"), PRINT_HELP);
	}

	//========================================================================================================================
	//
	//========================================================================================================================
	void handleOpen (TAsyncRequest * request)
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
		TAsyncResponseStream * response = request->beginResponseStream(F("application/json"));
		JsonObject & jsonRsp = jsonBuffer.createObject();

		jsonRsp["command"] = "open";
		jsonRsp["status"] = true;

		jsonRsp.prettyPrintTo(*response);
		request->send(response);
	}

	//========================================================================================================================
	//
	//========================================================================================================================
	void handleClose (TAsyncRequest * request)
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
		TAsyncResponseStream * response = request->beginResponseStream(F("application/json"));
		JsonObject & jsonRsp = jsonBuffer.createObject();

		jsonRsp["command"] = "close";
		jsonRsp["status"] = true;

		jsonRsp.prettyPrintTo(*response);
		request->send(response);
	}

};











