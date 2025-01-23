//************************************************************************************************************************
// WiFiServersManager.cpp
// Version 1.0 August, 2018
// Author Gerald Guiony
//************************************************************************************************************************

//#include <ESP8266mDNS.h>								// MDNS

//needed for library
#include <DNSServer.h>

#ifdef ESP8266
#	include <ESP8266WebServer.h>
#elif defined(ESP32)
#	include <WebServer.h>
#endif

#include <WiFiManager.h>								//https://github.com/tzapu/WiFiManager  =>  hotfixes branch !!!

// includes necessaires au fonctionnement de l'OTA :
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//#include <Ticker.h>
#include <StreamString.h>

#include <Common.h>

#include "DeviceDetectorServer.h"
#include "WiFiServersManager.h"
#include "LoggerTelnetServer.h"


DNSServer dnsSrv;


#ifdef ESP8266
// Les instances ne doivent pas etre détruite (shared_ptr) pour que les Event puissent être gérés
WiFiEventHandler wifiConnectedHandler;
WiFiEventHandler wifiDisconnectedHandler;

WiFiEventHandler stationConnectedHandler;
WiFiEventHandler stationDisconnectedHandler;
#endif


//Ticker wifiReconnectTimer;

// volatile because changed by ISR
// Volatile car mis a jour dans une interruption
volatile bool wifiManagerForcedByUser = false;


//========================================================================================================================
//
//========================================================================================================================
void WiFiServersManager :: setWifiManagerEnabled (bool enabled, bool accessPointIfNoWifi /* = false */) {
	_isWifiManagerEnabled = enabled;
	_accessPointIfNoWifi = accessPointIfNoWifi;
}

//========================================================================================================================
// Gets called when WiFiManager enters configuration mode
//========================================================================================================================
void configModeCallback (WiFiManager * wiFiManager) {

	Logln(
	F("Entered config mode: ")			<<
	WiFi.softAPIP() 					<< F(", ") <<
	wiFiManager->getConfigPortalSSID()	<< LN);			// If you used auto generated SSID, print it
}

//========================================================================================================================
//
//========================================================================================================================
bool WiFiServersManager :: startWiFiManager () {

	Logln(F("Starting WiFiManager.."));

	// Setup the DNS server redirecting all the domains to the apIP
	WiFiManager wifiManager;							// Local intialization. Once its business is done, there is no need to keep it around

//	wifiManager.resetSettings();						// Reset settings - for testing

//#ifndef DEBUG
	wifiManager.setDebugOutput(false);
//#endif
//	wifiManager.setConnectTimeout(30);					// sets timeout for which to attempt connecting, usefull if you get a lot of failed connects
	wifiManager.setTimeout(120);						// Sets timeout until configuration portal gets turned off useful to make it all retry or go to sleep in seconds
	wifiManager.setAPCallback(configModeCallback);		// Set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode

	// Fetches ssid and pass and tries to connect
	// If it does not connect it starts an access point with the specified name And goes into a blocking loop awaiting configuration
	bool result = wifiManager.autoConnect();

	if (result) {
		Logln(F("Wifi credential ok => Restart the chip"));
		EspBoard::reboot ();
	}
	else {
		Logln(F("Failed to connect and hit timeout"));
		if (_accessPointIfNoWifi) {
			WiFiHelper::startWiFiAccessPoint ();		// Avoid run in mixed mode (WIFI_AP_STA) because it causes strange behavior..
														// L'alternance entre mode AP et STA provoque des deconnexions en mode AP
		}
		else {
			EspBoard::reboot ();
		}
	}
	return result;
}

//========================================================================================================================
//
//========================================================================================================================
void WiFiServersManager :: startOTA () {

	Logln(F("Starting OTA.."));

	// Port defaults to 8266
	// ArduinoOTA.setPort(8266);
	// Hostname defaults to esp8266-[ChipID]
	ArduinoOTA.setHostname(EspBoard::getDeviceName().c_str());
	// No authentication by default
	// ArduinoOTA.setPassword((const char *)"123");

	ArduinoOTA.onStart([this]() {
		Logln(F("* OTA: Start"));
		notifyArduinoOTAStart ();
	});
	ArduinoOTA.onEnd([this]() {
		Logln(LN << F("*OTA: End"));
		notifyArduinoOTAEnd ();
	});
	ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
		Logln(F("*OTA: Progress: ") << (progress / (total / 100)) << F("%"));
		notifyArduinoOTAProgress (progress, total);
	});
	ArduinoOTA.onError([](ota_error_t error) {
		Logln(F("*OTA: Error[") << error << F("]: "));
		if (error == OTA_AUTH_ERROR)			Logln(F("Auth Failed"));
		else if (error == OTA_BEGIN_ERROR)		Logln(F("Begin Failed"));
		else if (error == OTA_CONNECT_ERROR)	Logln(F("Connect Failed"));
		else if (error == OTA_RECEIVE_ERROR)	Logln(F("Receive Failed"));
		else if (error == OTA_END_ERROR)		Logln(F("End Failed"));
	});

	ArduinoOTA.begin();						// initialisation de l'OTA
}

//========================================================================================================================
//
//========================================================================================================================
void WiFiServersManager :: startAllServers () {

	Logln(F("Starting Wifi servers.."));

	// Multicast DNS
	// http://onlineshouter.com/assign-web-address-esp8266-using-multicast-dns/
	// Set up mDNS responder:
	// - first argument is the domain name, in this example
	//   the fully-qualified domain name is "espXXXXXXX.local"
	// - second argument is the IP address to advertise
	//   we send our IP address on the WiFi network
	//
	// - Install host software:
	// - For Linux, install Avahi (http://avahi.org/).
	// - For Windows, install Bonjour (http://www.apple.com/support/bonjour/).
	// - For Mac OSX and iOS support is built in through Bonjour already.
	// - Point your browser to http://espXXXXXXX.local, you should see a response
	// server accessible via http://espXXXXXXX.local, URL thanks to mDNS responder
//	if (MDNS.begin(EspBoard::getDeviceName().c_str())) {
//		Logln(F("* MDNS responder started. Hostname -> ") << EspBoard::getDeviceName());
//	}

	startOTA ();

#ifdef DEBUG
	I(LoggerTelnetServer).setup ();
#endif

	if (!WiFiHelper::isAccessPointMode()) {
		// Only when the device is connected to Wifi in station mode
		I(DeviceDetectorServer).setup ();
	}

	// Start other servers here ...
	startCustomServers ();

	// Lastly (after servers started), use the MDNS built-in object to call the method Add service
	// Add service to MDNS-SD
	// This requires three parameters. These parameters describe the type of service that we want to broadcast over
	// the network. In this case, we will use HTTP over TCP on 80.
//	MDNS.addService("http", "tcp", 80);

	// start dns server
	if (!dnsSrv.start(53, EspBoard::getDeviceName (), WiFiHelper :: getIpAddress ())) {
		Logln(F("Failed to start dns service"));
	}
}

//========================================================================================================================
//
//========================================================================================================================
void WiFiServersManager :: stopAllServers () {

	Logln(F("Stop Wifi servers"));

#ifdef DEBUG
	I(LoggerTelnetServer).stop ();
#endif

	I(DeviceDetectorServer).stop();

	// Stop other servers...
	stopCustomServers();
}

//========================================================================================================================
//
//========================================================================================================================
void WiFiServersManager :: startWifi () {

	if (_forceAccessPoint) {
		Logln (F("Starting wifi access point.."));
		// Force access point mode
		WiFiHelper::startWiFiAccessPoint ();
	}
	else {
		Logln (F("Starting wifi.."));
		// Default setup : connect device to wifi in station mode otherwise start wifiManager ..
		if (!WiFiHelper::connectToWiFi ()) {
			if ((_isWifiManagerEnabled)||(wifiManagerForcedByUser)) {
				wifiManagerForcedByUser = false;
				startWiFiManager ();
			}
		}
	}
}

//========================================================================================================================
//
//========================================================================================================================
void WiFiServersManager :: startWifiAndServers () {

	EspBoard::blinkOn();									// Led on

	startWifi ();

	if (WiFiHelper::isWifiAvailable ()) {
		Logln (F("Chip IP Address: ") << WiFiHelper :: getIpAddress ());
		startAllServers ();
	}
	else {
		Logln (F("No Wifi available !"));
		//	enter in deepsleep or reboot ?
	}

	EspBoard::blinkOff();									// Led off
}

//========================================================================================================================
// Interrupt Service Routines (ISR) handler has to be marked with ICACHE_RAM_ATTR
//========================================================================================================================
#if defined (ESP8266) || defined (ESP32)
void IRAM_ATTR _ISR_user_btn ()
#else
void _ISR_user_btn ()
#endif
{
//	Logln("User button state:" << digitalRead());

	// Si le device ne peut pas se reconnecter au wifi, l'utilisateur peut autoriser le wifiManager a se lancer en appuyant
	// sur le boutton USER_BTN (notament lors d'un reboot après un deep sleep)
	wifiManagerForcedByUser = true;

	EspBoard::blinkOn();	// Led on
}

#ifdef ESP32
//========================================================================================================================
//
//========================================================================================================================
static void logWiFiEvent(WiFiEvent_t event)
{
	Logln (F("[WiFi-event] event: ") << event);

	switch (event) {
		case ARDUINO_EVENT_WIFI_READY:			  	Logln (F("WiFi interface ready")); break;
		case ARDUINO_EVENT_WIFI_SCAN_DONE:		  	Logln (F("Completed scan for access points")); break;
		case ARDUINO_EVENT_WIFI_STA_START:		  	Logln (F("WiFi client started")); break;
		case ARDUINO_EVENT_WIFI_STA_STOP:		   	Logln (F("WiFi clients stopped")); break;
		case ARDUINO_EVENT_WIFI_STA_CONNECTED:	  	Logln (F("Connected to access point")); break;
		case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:   Logln (F("Disconnected from WiFi access point")); break;
		case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:Logln (F("Authentication mode of access point has changed")); break;
		case ARDUINO_EVENT_WIFI_STA_GOT_IP: 		Logln (F("Obtained IP address: ")); break;
		case ARDUINO_EVENT_WIFI_STA_LOST_IP:		Logln (F("Lost IP address and IP address is reset to 0")); break;
		case ARDUINO_EVENT_WPS_ER_SUCCESS:		  	Logln (F("WiFi Protected Setup (WPS): succeeded in enrollee mode")); break;
		case ARDUINO_EVENT_WPS_ER_FAILED:		   	Logln (F("WiFi Protected Setup (WPS): failed in enrollee mode")); break;
		case ARDUINO_EVENT_WPS_ER_TIMEOUT:		  	Logln (F("WiFi Protected Setup (WPS): timeout in enrollee mode")); break;
		case ARDUINO_EVENT_WPS_ER_PIN:			  	Logln (F("WiFi Protected Setup (WPS): pin code in enrollee mode")); break;
		case ARDUINO_EVENT_WIFI_AP_START:		   	Logln (F("WiFi access point started")); break;
		case ARDUINO_EVENT_WIFI_AP_STOP:			Logln (F("WiFi access point  stopped")); break;
		case ARDUINO_EVENT_WIFI_AP_STACONNECTED:	Logln (F("Client connected")); break;
		case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:	Logln (F("Client disconnected")); break;
		case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:   Logln (F("Assigned IP address to client")); break;
		case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:  Logln (F("Received probe request")); break;
		case ARDUINO_EVENT_WIFI_AP_GOT_IP6:		 	Logln (F("AP IPv6 is preferred")); break;
		case ARDUINO_EVENT_WIFI_STA_GOT_IP6:		Logln (F("STA IPv6 is preferred")); break;
		case ARDUINO_EVENT_ETH_GOT_IP6:			 	Logln (F("Ethernet IPv6 is preferred")); break;
		case ARDUINO_EVENT_ETH_START:			   	Logln (F("Ethernet started")); break;
		case ARDUINO_EVENT_ETH_STOP:				Logln (F("Ethernet stopped")); break;
		case ARDUINO_EVENT_ETH_CONNECTED:		   	Logln (F("Ethernet connected")); break;
		case ARDUINO_EVENT_ETH_DISCONNECTED:		Logln (F("Ethernet disconnected")); break;
		case ARDUINO_EVENT_ETH_GOT_IP:			  	Logln (F("Obtained IP address")); break;
		default:									break;
	}
}
#endif

//========================================================================================================================
//
//========================================================================================================================
void WiFiServersManager :: setup (bool forceAccessPoint /*= false */) {

	_forceAccessPoint = forceAccessPoint;

#ifdef ARDUINO_ESP8266_WIO_NODE
	WiFiHelper::resetWiFiHardware ();
#endif

	// Host name of WiFi
	// Hostname defaults to esp8266-[FIN DE LA MAC ADDRESS]
	Logln (F("Chip Hostname: ") << EspBoard::getDeviceName());
	WiFi.hostname (EspBoard::getDeviceName().c_str());

#ifdef ESP8266

	// Register event handlers.
	// Callback functions will be called as long as these handler objects exist.
	wifiConnectedHandler 		= WiFi.onStationModeGotIP (
		[this] (const WiFiEventStationModeGotIP& event) {
			Logln (F("Wifi connected: ") << event.ip);
			notifyConnectedToWiFi (event.ip);
		});
	wifiDisconnectedHandler 	= WiFi.onStationModeDisconnected (
		[this] (const WiFiEventStationModeDisconnected& ) {
			Logln (F("Wifi disconnected!"));
			notifyDisconnectedFromWiFi ();
		});
	stationConnectedHandler		= WiFi.onSoftAPModeStationConnected	(
		[this] (const WiFiEventSoftAPModeStationConnected& event) {
			const unsigned char* mac = event.mac;
			Logln (F("Client connected: ")	<< mac[0] << F(":") << mac[1] << F(":") << mac[2] << F(":")
											<< mac[3] << F(":") << mac[4] << F(":") << mac[5]);
			notifyClientConnected ();
		});
	stationDisconnectedHandler	= WiFi.onSoftAPModeStationDisconnected (
		[this] (const WiFiEventSoftAPModeStationDisconnected& event) {
			const unsigned char* mac = event.mac;
			Logln (F("Client disconnected: ")	<< mac[0] << F(":") << mac[1] << F(":") << mac[2] << F(":")
												<< mac[3] << F(":") << mac[4] << F(":") << mac[5]);
			notifyClientDisconnected ();
		});

#elif defined (ESP32)

	WiFi.onEvent(logWiFiEvent);

	WiFi.onEvent(
		[this](WiFiEvent_t event, WiFiEventInfo_t info) {
  			Logln (F("WiFi connected, IP address: "));
  			Logln (IPAddress(info.got_ip.ip_info.ip.addr));
			notifyConnectedToWiFi (info.got_ip.ip_info.ip.addr);
		},
		WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);

	WiFi.onEvent(
		[this](WiFiEvent_t event, WiFiEventInfo_t info) {
	  		Logln (F("WiFi lost connection. Reason: "));
	  		Logln (info.wifi_sta_disconnected.reason);
			notifyDisconnectedFromWiFi ();
		},
		WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

	WiFi.onEvent(
		[this](WiFiEvent_t event, WiFiEventInfo_t info) {
	  		Logln (F("Client connected: "));
 			Logln (IPAddress(info.wifi_ap_staipassigned.ip.addr));
			notifyClientConnected ();
		},
		WiFiEvent_t::ARDUINO_EVENT_WIFI_AP_STACONNECTED);

	WiFi.onEvent(
		[this](WiFiEvent_t event, WiFiEventInfo_t info) {
	  		Logln (F("Client disconnect: "));
	  		//Logln (info.wifi_ap_stadisconnected);
			notifyClientDisconnected ();
		},
		WiFiEvent_t::ARDUINO_EVENT_WIFI_AP_STADISCONNECTED);

#endif



	if (!_isWifiManagerEnabled) {
		pinMode(USER_BTN, INPUT);
		attachInterrupt(digitalPinToInterrupt(USER_BTN), _ISR_user_btn, CHANGE);
	}

	startWifiAndServers ();
}

//========================================================================================================================
//
//========================================================================================================================
void WiFiServersManager :: loop () {


	if (WiFiHelper::isWifiAvailable()) {

		dnsSrv.processNextRequest();

		// A chaque iteration, on verifie si une mise a jour nous est envoyee
		// si tel est cas, la lib ArduinoOTA se charge de gerer la suite :)
		ArduinoOTA.handle();

		if (!WiFiHelper::isAccessPointMode()) {
			I(DeviceDetectorServer).loop();
		}
	}
}

