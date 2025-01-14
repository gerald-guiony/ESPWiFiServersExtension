//************************************************************************************************************************
// HttpAdminCommandRequestHandler.cpp
// Version 1.0 August, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#include <ArduinoJson.h>

#include <AsyncJson.h>

#include "HttpAdminCommandRequestHandler.h"



// Exemples json : https://reqres.in/

// HTTP Request Methods
// https://www.w3schools.com/tags/ref_httpmethods.asp



// Use PROGMEM and server.send_P to send data from PROGMEM — in this case it doesn't need to be copied to RAM twice, you avoid allocating Strings and all the associated issues.
static const char PROGMEM PRINT_HELP[] = R"rawliteral(
===========================================================================================================
 *** AVAILABLE ADMINISTRATIVE JSON COMMANDS ON THIS DEVICE ***
===========================================================================================================

-----------------------------------------------------------------------------------------------------------
 Json command format :                    /admin/$cmd?json={"$name":"$value", ..}
-----------------------------------------------------------------------------------------------------------

 Supported commands list ($cmd?json={"$name":"$value", ..}) :

 help ................................... Print this help
 info ................................... Device informations
 reboot ................................. Reboot the device
 listallfiles ........................... List all spiffs files
 delallfiles ............................ Delete all spiffs files (Warning : All IR signals will be lost !)
 wifi : {"ssid":"$1", "pass":"$2"} ...... WiFi ssid and password to connect this iot device to the Network

===========================================================================================================
)rawliteral";




SINGLETON_IMPL (HttpAdminCommandRequestHandler)


//========================================================================================================================
//
//========================================================================================================================
void HttpAdminCommandRequestHandler :: handleHelp (AsyncWebServerRequest * request)
{
	Logln (F("=> help"));
	request->send_P(200, F("text/plain"), PRINT_HELP);
}

//========================================================================================================================
//
//========================================================================================================================
void HttpAdminCommandRequestHandler :: handleInfo (AsyncWebServerRequest * request)
{
	Logln (F("=> info"));

	// -----------------------------
	// ArduinoJson Advanced Response
	// https://github.com/me-no-dev/ESPAsyncWebServer#arduinojson-advanced-response
	// This response can handle really large Json objects (tested to 40KB) There isn't any noticeable speed decrease for small results
//	AsyncJsonResponse * response = new AsyncJsonResponse();
	PrettyAsyncJsonResponse * response = new PrettyAsyncJsonResponse();
	JsonObject& jsonRsp = response->getRoot();

	jsonRsp["deviceName"]		= EspBoard::getDeviceName ();
	jsonRsp["chipId"]			= EspBoard::getDeviceId ();
	jsonRsp["timeSinceBoot"]	= EspBoard::getTimeElapsedSinceBoot ();
	jsonRsp["heap"]				= ESP.getFreeHeap ();
#ifdef ESP8266
	jsonRsp["lastResetReason"]	= EspBoard::getResetReason ();
#elif defined (ESP32)
	jsonRsp["resetReason cpu0"]	= EspBoard::getResetReason (0);
	jsonRsp["resetReason cpu1"]	= EspBoard::getResetReason (1);
#endif
	jsonRsp["coreVersion"]		= ESP.getCoreVersion ();
	jsonRsp["sdkVersion"]		= ESP.getSdkVersion ();
	jsonRsp["cpuFreq"]			= ESP.getCpuFreqMHz ();
	jsonRsp["sketchSize"]		= ESP.getSketchSize ();
	jsonRsp["freeSketchSpace"]	= ESP.getFreeSketchSpace ();
	jsonRsp["freeSpiffsSpace"]	= FileStorage::spiffsCheckRemainingBytes ();
	jsonRsp["accessPoint"]		= WiFiHelper::isAccessPointMode ();
	jsonRsp["IP"]				= WiFiHelper::getIpAddress ().toString();
	jsonRsp["macAddress"]		= WiFiHelper::getMacAddress ();
	jsonRsp["subnetMask"]		= WiFi.subnetMask().toString();
	jsonRsp["gatewayIP"]		= WiFi.gatewayIP().toString();
	jsonRsp["dnsIP"]			= WiFi.dnsIP().toString();
#ifdef ESP826
	jsonRsp["hostname"]			= WiFi.hostname();
#elif defined (ESP32)
	jsonRsp["hostname"]			= WiFi.getHostname();
#endif
	jsonRsp["SSID"]				= WiFi.SSID();
	jsonRsp["BSSID"]			= WiFi.BSSIDstr();
	jsonRsp["RSSI"]				= WiFi.RSSI();

	response->setLength();
	request->send(response);
}

//========================================================================================================================
//
//========================================================================================================================
void HttpAdminCommandRequestHandler :: handleReboot (AsyncWebServerRequest * request)
{
	Logln (F("=> reboot"));

	// -----------------------------
	// ArduinoJson Basic Response
	// https://github.com/me-no-dev/ESPAsyncWebServer#arduinojson-basic-response
	// This way of sending Json is great for when the result is below 4KB
	AsyncResponseStream *response = request->beginResponseStream(F("application/json"));

	// Memory pool for JSON object tree.
	// Inside the brackets, 200 is the size of the pool in bytes.
	// Don't forget to change this value to match your JSON document.
	// Use arduinojson.org/assistant to compute the capacity.
	//StaticJsonBuffer<200> jsonBuffer;

	// StaticJsonBuffer allocates memory on the stack, it can be
	// replaced by DynamicJsonBuffer which allocates in the heap.
	DynamicJsonBuffer jsonBuffer;

	// Create the root of the object tree.
	// It's a reference to the JsonObject, the actual bytes are inside the
	// JsonBuffer with all the other nodes of the object tree.
	// Memory is freed when jsonBuffer goes out of scope.
	JsonObject& jsonRsp = jsonBuffer.createObject();

	jsonRsp["command"] = "reboot";
	jsonRsp["status"] = true;
	jsonRsp["message"] = "The device is rebooting..";

	jsonRsp.prettyPrintTo(*response);
	request->send(response);

	I(ModuleSequencer).requestReboot ();
}

//========================================================================================================================
//
//========================================================================================================================
void HttpAdminCommandRequestHandler :: handleListAllFiles (AsyncWebServerRequest * request)
{
	Logln (F("=> List all files"));

	// This way of sending Json is great for when the result is below 4KB
	AsyncResponseStream *response = request->beginResponseStream(F("application/json"));
	DynamicJsonBuffer jsonBuffer;
	JsonObject& jsonRsp = jsonBuffer.createObject();

	String listFiles = FileStorage::spiffsListFiles ();

	jsonRsp["command"] = "listallfiles";
	jsonRsp["response"] = (listFiles.length () > 0) ? listFiles : "No file found";

	jsonRsp.prettyPrintTo(*response);
	request->send(response);
}

//========================================================================================================================
//
//========================================================================================================================
void HttpAdminCommandRequestHandler :: handleDelAllFiles (AsyncWebServerRequest * request)
{
	Logln (F("=> delete all files"));

	// This way of sending Json is great for when the result is below 4KB
	AsyncResponseStream *response = request->beginResponseStream(F("application/json"));
	DynamicJsonBuffer jsonBuffer;
	JsonObject& jsonRsp = jsonBuffer.createObject();

	FileStorage::spiffsRemoveAllFiles ();

	jsonRsp["command"] = "delallfiles";
	jsonRsp["status"] = true;

	jsonRsp.prettyPrintTo(*response);
	request->send(response);
}

//========================================================================================================================
//
//========================================================================================================================
void HttpAdminCommandRequestHandler :: handleWifi (AsyncWebServerRequest * request)
{
	Logln (F("=> set wifi ssid + pass"));

	if (!request->hasArg("json")) {
		request->send(400, F("text/plain"), F("400: json argument not found"));				// The request is invalid, so send HTTP status 400
		return;
	}

	DynamicJsonBuffer jsonBuffer;
	JsonObject& jsonArg = jsonBuffer.parse(request->arg("json"));

	// Test if parsing succeeds.
	if (!jsonArg.success()) {
		request->send(400, F("text/plain"), F("400: Invalid json argument"));
		return;
	}

	if (!jsonArg ["ssid"].success() || !jsonArg ["pass"].success()) {
		request->send(400, F("text/plain"), F("400: Invalid json field"));
		return;
	}

	String ssid = jsonArg ["ssid"];
	String pass = jsonArg ["pass"];

	WiFi.begin (ssid.c_str(), pass.c_str());		// Serialization automatique du ssid et password
	EspBoard::asyncDelayMillis (1000);

	// This way of sending Json is great for when the result is below 4KB
	AsyncResponseStream *response = request->beginResponseStream(F("application/json"));
	JsonObject& jsonRsp = jsonBuffer.createObject();

	jsonRsp["command"] = "wifi";
	jsonRsp["status"] = true;
	jsonRsp["message"] = "The device is rebooting for these changes to take effect";

	jsonRsp.prettyPrintTo(*response);
	request->send(response);

	I(ModuleSequencer).requestReboot ();
}

//========================================================================================================================
//
//========================================================================================================================
void HttpAdminCommandRequestHandler :: setup (AsyncWebServer & asyncWebServer)
{
	asyncWebServer.on("/admin/help",			std::bind(&HttpAdminCommandRequestHandler::handleHelp,			this, _1));
	asyncWebServer.on("/admin/info",			std::bind(&HttpAdminCommandRequestHandler::handleInfo,			this, _1));
	asyncWebServer.on("/admin/reboot",			std::bind(&HttpAdminCommandRequestHandler::handleReboot,		this, _1));
	asyncWebServer.on("/admin/listallfiles",	std::bind(&HttpAdminCommandRequestHandler::handleListAllFiles,	this, _1));
	asyncWebServer.on("/admin/delallfiles",		std::bind(&HttpAdminCommandRequestHandler::handleDelAllFiles,	this, _1));
	asyncWebServer.on("/admin/wifi",			std::bind(&HttpAdminCommandRequestHandler::handleWifi,			this, _1));
}

