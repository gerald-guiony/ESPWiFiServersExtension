//************************************************************************************************************************
// MqttDomoticzClient.h
// Version 1.0 Jan, 2025
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include "MqttClientAsync.h"

namespace wifix {

//------------------------------------------------------------------------------
//
class MqttDomoticzClient : public MqttClientAsync
{
	SINGLETON_CLASS(MqttDomoticzClient)
};

}