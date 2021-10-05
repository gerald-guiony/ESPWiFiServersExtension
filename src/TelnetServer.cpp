//************************************************************************************************************************
// TelnetServer.cpp
// Version 2.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************


#include <vector>
#include <algorithm>

#include <Common.h>

#include "TelnetServer.h"


std::vector <AsyncClient *> clients; // a list to hold all clients


//========================================================================================================================
//
//========================================================================================================================
TelnetServer :: TelnetServer () {
}

//========================================================================================================================
//
//========================================================================================================================
TelnetServer :: ~TelnetServer () {
	stop ();
	if (_server) {
		delete _server;
		_server = NULL;
	}
}

//========================================================================================================================
//
//========================================================================================================================
void TelnetServer :: stop () {

	for (AsyncClient * client : clients) {
		if (client != nullptr) {
			client->close(true);
//			delete client;
		}
	}

	clients.clear ();

	// Stop server
	if (_server) {
//		_server->end();
	}
}

//========================================================================================================================
// server events
//========================================================================================================================
void TelnetServer :: onNewClient (void* arg, AsyncClient* client) {

	Logln(F("New client has been connected to server, ip: ") << client->remoteIP());

	notifyClientConnected (client);

	clients.push_back (client);

	// register events
	client->onData (
		[this] (void* arg, AsyncClient* client, void *data, size_t len) {
			//	Logln(F("Data received from client: ") << client->remoteIP());
			notifyClientDataReceived (client, data, len);
		},
		NULL);
	client->onError (
		[this] (void* arg, AsyncClient* client, int8_t error) {
			Logln(F("Connection error: ") << client->errorToString(error) << F(" from client: ") << client->remoteIP());
			notifyClientError (client, error);
		},
		NULL);
	client->onDisconnect (
		[this] (void* arg, AsyncClient* client) {
			//	Logln(F("Client disconnected: ") << client->remoteIP());
			notifyClientDisconnected (client);

			auto it = std::find (clients.begin(), clients.end(), client);
			if (it != clients.end()) {
				clients.erase (it);
				delete client;
			}
		},
		NULL);
	client->onTimeout (
		[this] (void* arg, AsyncClient* client, uint32_t time) {
			Logln(F("Client ACK timeout ip: ") << client->remoteIP());
			notifyClientTimeout (client, time);
		},
		NULL);
}

//========================================================================================================================
//
//========================================================================================================================
void TelnetServer :: setup (uint16_t thePort /* = DEFAULT_TELNET_PORT*/) {

	_server = new AsyncServer (thePort); // start listening on tcp port

//	_server->setNoDelay (true);

	_server->onClient (std::bind(&TelnetServer::onNewClient, this, std::placeholders::_1, std::placeholders::_2), NULL);
	_server->begin ();

	Logln(F("Telnet server is up"));
	Logln(F("Ready! Use 'telnet ") << WiFiHelper::getIpAddress() << F(":") << thePort << F(" to connect"));
}

//========================================================================================================================
//
//========================================================================================================================
void TelnetServer :: loop () {
}