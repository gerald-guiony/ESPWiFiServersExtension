//************************************************************************************************************************
// DeviceDetectorClient.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>

#include "UdpClient.h"

//------------------------------------------------------------------------------
// Singleton
class DeviceDetectorClient : public UdpClient
{
	SINGLETON_CLASS(DeviceDetectorClient)

private:
	virtual StreamParser & getRespParser		() override;

public:
	void sendDeviceDetectionCmd 				(int port);
};
