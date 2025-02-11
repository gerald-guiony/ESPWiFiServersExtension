//************************************************************************************************************************
// HttpRelayCommandRequestHandlerAbstract.h
// Version 1.0 December, 2025
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <ArduinoJson.h>

#include <Common.h>
#include <Switches/BasicRelay.h>

using namespace corex;


extern BasicRelay basicRelay;


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
 open ............................ Open the relay
 close ........................... Close the relay

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

		// Visual indicator that signal sent
		EspBoard::blinks (3);

		basicRelay.open();

		// StaticJsonBuffer allocates memory on the stack, it can be
		// replaced by DynamicJsonBuffer which allocates in the heap.
		DynamicJsonBuffer jsonBuffer;

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

		// Visual indicator that signal sent
		EspBoard::blinks (5);

		basicRelay.close();

		// StaticJsonBuffer allocates memory on the stack, it can be
		// replaced by DynamicJsonBuffer which allocates in the heap.
		DynamicJsonBuffer jsonBuffer;

		// This way of sending Json is great for when the result is below 4KB
		TAsyncResponseStream * response = request->beginResponseStream(F("application/json"));
		JsonObject & jsonRsp = jsonBuffer.createObject();

		jsonRsp["command"] = "close";
		jsonRsp["status"] = true;

		jsonRsp.prettyPrintTo(*response);
		request->send(response);
	}

};











