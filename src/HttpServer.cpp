//************************************************************************************************************************
// HttpServer.cpp
// Version 1.0 August, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#include <Stream.h>
#include <StreamString.h>


#include "HttpServer.h"


namespace wifix {

static AsyncWebServer			asyncWebServer (80);
static HttpAsyncWebHandler		httpAsyncWebHandler;


SINGLETON_IMPL (HttpServer)

//========================================================================================================================
//
//========================================================================================================================
void HttpServer :: handleNotFound (AsyncWebServerRequest *request)
{
	Logln(F("handleNotFound"));

	if (isCaptivePortal(request)) {					// If captive portal redirect instead of displaying the error page.
		return;
	}

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
	AsyncWebServerResponse * response = request->beginResponse(404, "text/plain", sstr);
	response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
	response->addHeader("Pragma", "no-cache");
	response->addHeader("Expires", "-1");
	request->send (response);

	Logln(sstr);
}

//========================================================================================================================
// Le portail captif est une technique consistant à forcer les clients HTTP d'un réseau de consultation (wifi) à afficher
// une page web spéciale (le plus souvent dans un but d'authentification) avant d'accéder à Internet normalement.
// Redirect to captive portal if we got a request for another domain. Return true in that case so the page handler do not
// try to handle the request again.
//========================================================================================================================
bool HttpServer :: isCaptivePortal (AsyncWebServerRequest *request) {

	IPAddress ip;

	if (!ip.fromString (request->host())) {	// "clients3.google.com/generate_204" for ex..
		Logln(F("Request redirected to captive portal : ") << request->host() << request->url());

		// https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
		// 302 Found (Previously "Moved temporarily")
		AsyncWebServerResponse * response = request->beginResponse(302, "text/plain", "Found (Previously \"Moved temporarily\")");
		response->addHeader("Location", String("http://") + request->client()->localIP().toString());
		request->send(response);
		request->client()->stop();	// Stop is needed because we sent no content length

		return true;
	}
	return false;
}

//========================================================================================================================
//
//========================================================================================================================
void HttpServer :: addRequestHandlers	(const std::vector <HttpRequestHandler *> & hanlders)
{
	for (HttpRequestHandler * handler : hanlders) {
		handler->setup (asyncWebServer);
	}
}

//========================================================================================================================
//
//========================================================================================================================
void HttpServer :: connect ()
{
	asyncWebServer.begin();

	Logln(F("HTTP Web server is up"));
	Logln(F("Open http://") << WiFiHelper::getIpAddress() << F("/ in your browser to see it working"));
}


//========================================================================================================================
//
//========================================================================================================================
void HttpServer :: disconnect ()
{
	asyncWebServer.end ();  // remove all writers and handlers, with onNotFound/onFileUpload/onRequestBody
}

//========================================================================================================================
//
//========================================================================================================================
void HttpServer :: setup (const std::vector <HttpRequestHandler *> & handlers) {

	addRequestHandlers (handlers);

	//_asyncWebServer->on("/generate_204", std::bind(&WebServer::handle204, this, std::placeholders::_1));  	// Android/Chrome OS captive portal check (par ex dans le cas où on connecte l'esp8266 au wifi d'un Android)
	//_asyncWebServer->on("/fwlink", std::bind(&WebServer::handleRoot, this, std::placeholders::_1)); 			// Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
	asyncWebServer.onNotFound (std::bind(&HttpServer::handleNotFound, this, std::placeholders::_1));			// Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
	asyncWebServer.addHandler (&httpAsyncWebHandler);
}

}


