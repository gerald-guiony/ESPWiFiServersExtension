//************************************************************************************************************************
// DeviceDetectorServer.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>

#include "UdpServer.h"

//------------------------------------------------------------------------------
// Singleton
class DeviceDetectorServer : public UdpServer
{
	SINGLETON_CLASS(DeviceDetectorServer)

private:
	virtual StreamParser & getCmdParser			() override;
};

