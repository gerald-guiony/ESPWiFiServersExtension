//************************************************************************************************************************
// UdpDeviceDetectorClient.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>

#include "UdpClient.h"


namespace wifix {

//------------------------------------------------------------------------------
// Singleton
class UdpDeviceDetectorClient : public UdpClient
{
	SINGLETON_CLASS(UdpDeviceDetectorClient)

private:
	virtual StreamParser & getRespParser		() override;

public:
	void sendDeviceDetectionCmd 				(int port);
};

}