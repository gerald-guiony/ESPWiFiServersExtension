//************************************************************************************************************************
// UdpDeviceDetectorServer.cpp
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#include "UdpDeviceDetectorCommandParser.h"
#include "UdpDeviceDetectorServer.h"


namespace wifix {

SINGLETON_IMPL (UdpDeviceDetectorServer)

//========================================================================================================================
//
//========================================================================================================================
StreamParser & UdpDeviceDetectorServer :: getCmdParser () {
	return I(UdpDeviceDetectorCommandParser);
}

}