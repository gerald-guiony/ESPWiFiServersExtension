//************************************************************************************************************************
// UdpServer.cpp
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************


#ifdef ESP8266
#	include <ESP8266mDNS.h>					// MDNS
#elif defined(ESP32)
#	include <ESPmDNS.h>
#endif

#include <Common.h>

#include "UdpServer.h"

namespace wifix {

//========================================================================================================================
//
//========================================================================================================================
void UdpServer :: setup (uint16_t thePort) {

	_port = thePort;
	MDNS.addService("telnet", "udp", _port);
}

//========================================================================================================================
//
//========================================================================================================================
void UdpServer :: start () {
	// Start the server
	_wiFiUdp.begin (_port);

	Logln(F("Udp server is up"));
	Logln(F("Now listening at IP ") << WiFiHelper::getIpAddress() << F(", UDP port ") << _port);
}

//========================================================================================================================
//
//========================================================================================================================
void UdpServer :: stop () {
	_wiFiUdp.stop();
}

//========================================================================================================================
//
//========================================================================================================================
void UdpServer :: loop () {

	// Check if there are any new packets
	int packetSize = _wiFiUdp.parsePacket();
	if (packetSize)
	{
		notifyPacketReceived ();

		MemStream memStreamPrinter;
		getCmdParser().parse (_wiFiUdp, memStreamPrinter);

		// send back the reply, to the IP address and port we got the packet from
		_wiFiUdp.beginPacket (_wiFiUdp.remoteIP(), _wiFiUdp.remotePort());
		//_server.write (replyPacket);
		memStreamPrinter.writeTo (_wiFiUdp);
		_wiFiUdp.endPacket ();
	}
}

}