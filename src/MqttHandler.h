//************************************************************************************************************************
// MqttHandler.h
// Version 1.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <AsyncMqttClient.h>				// https://github.com/marvinroger/async-mqtt-client

#include <Module/AsyncModule.h>

using namespace std::placeholders;
using namespace corex;

namespace wifix {

//------------------------------------------------------------------------------
//
class MqttHandler : public AsyncModule <AsyncMqttClient * >
{
};

}


