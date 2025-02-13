//************************************************************************************************************************
// UdpDeviceDetectorCommandParser.cpp
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#ifdef ESP8266
#	include <ESP8266WiFi.h>					// WiFi.localIP()
#elif defined(ESP32)
#	include <WiFi.h>
#endif

#include "UdpDeviceDetectorCommandParser.h"


#define PRINT_HELP																											 \
	F("-------------------------------------------------------------------------------------------------------")	<< LN << \
	F(" *** AVAILABLE IOT DEVICE DETECTOR COMMANDS *** ")															<< LN << \
	F("-------------------------------------------------------------------------------------------------------")	<< LN << \
	F(">> [0] ................. Print this help")																	<< LN << \
	F(">> [1] ................. Name, Category, IP and Mac address of this IOT device")								<< LN << \
	F("-------------------------------------------------------------------------------------------------------")



namespace wifix {


SINGLETON_IMPL (UdpDeviceDetectorCommandParser)


//========================================================================================================================
//
//========================================================================================================================
bool UdpDeviceDetectorCommandParser :: parse (Stream & stream, Print & printer) {

	if (!checkCmdBegin (stream)) return false;

	Logln (LN);
	printer << LN;

	do {

		int commandId = getCmdId (stream);
		if (commandId < 0) {
			printer << PRINT_PARSE_FAILS("??") << LN;
			return false;
		}

		switch (commandId)
		{
		case 0:
			Logln(F("=> HELP"));
			printer << PRINT_RESP (commandId, LN << PRINT_HELP << LN) << LN;
			break;

		case 1:
			// echo -n ">> [1]" | socat -t60 - udp-datagram:192.168.1.255:555,broadcast
			Logln(F("=> NAME, CATEGORY, IP AND MAC ADDRESS"));

			printer << PRINT_RESP (commandId,
				EspBoard::getDeviceName()											<< F(MSG_SEPARATOR_PARAM) <<
				WiFi.localIP()														<< F(MSG_SEPARATOR_PARAM) <<
				WiFi.macAddress()
				) << LN;
			break;

		default:
			printer << F("Command not available") << LN;
			printer << PRINT_NACK (commandId) << LN;
			return false;
		}

	} while (checkSeparatorCmd (stream));

	return true;
}

}








