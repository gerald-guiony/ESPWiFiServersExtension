//************************************************************************************************************************
// DeviceDetectorClient.cpp
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#include "DeviceDetectorResponseParser.h"
#include "DeviceDetectorClient.h"


SINGLETON_IMPL (DeviceDetectorClient)


//========================================================================================================================
//
//========================================================================================================================
void DeviceDetectorClient :: sendDeviceDetectionCmd (int port) {
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
StreamParser & DeviceDetectorClient :: getRespParser () {
	return I(DeviceDetectorResponseParser);
}

