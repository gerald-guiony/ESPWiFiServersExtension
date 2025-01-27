//************************************************************************************************************************
// UdpDeviceDetectorClient.cpp
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#include "UdpDeviceDetectorResponseParser.h"
#include "UdpDeviceDetectorClient.h"


namespace wifix {

SINGLETON_IMPL (UdpDeviceDetectorClient)

//========================================================================================================================
//
//========================================================================================================================
void UdpDeviceDetectorClient :: sendDeviceDetectionCmd (int port) {
	MemStream mem;
	mem << PRINT_CMD(1);
	String cmd = mem.readString();

	IPAddress ip = WiFiHelper::getIpAddress();

	Logln (F("Send command: \"") << cmd << F("\" to ") << ip << F(":") << port);

	sendCmd (ip, port, cmd);
}

//========================================================================================================================
//
//========================================================================================================================
StreamParser & UdpDeviceDetectorClient :: getRespParser () {
	return I(UdpDeviceDetectorResponseParser);
}

}