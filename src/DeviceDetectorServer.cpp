//************************************************************************************************************************
// DeviceDetectorServer.cpp
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#include "DeviceDetectorCommandParser.h"
#include "DeviceDetectorServer.h"


SINGLETON_IMPL (DeviceDetectorServer)

//========================================================================================================================
//
//========================================================================================================================
StreamParser & DeviceDetectorServer :: getCmdParser () {
	return I(DeviceDetectorCommandParser);
}

