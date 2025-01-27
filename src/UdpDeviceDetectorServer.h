//************************************************************************************************************************
// UdpDeviceDetectorServer.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>

#include "UdpServer.h"


namespace wifix {

//------------------------------------------------------------------------------
// Singleton
class UdpDeviceDetectorServer : public UdpServer
{
	SINGLETON_CLASS(UdpDeviceDetectorServer)

private:
	virtual StreamParser & getCmdParser			() override;
};

}