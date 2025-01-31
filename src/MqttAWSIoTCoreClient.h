//************************************************************************************************************************
// MqttAWSIoTCoreClient.h
// Version 1.0 Jan, 2025
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include "MqttClientCredential.h"

namespace wifix {

//------------------------------------------------------------------------------
//
class MqttAWSIoTCoreClient : public MqttClientCredential
{
	SINGLETON_CLASS(MqttAWSIoTCoreClient)
};

}