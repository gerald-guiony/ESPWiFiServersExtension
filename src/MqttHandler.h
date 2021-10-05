//************************************************************************************************************************
// MqttHandler.h
// Version 1.0 December, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <AsyncMqttClient.h>

using namespace std::placeholders;

//------------------------------------------------------------------------------
//
class MqttHandler
{
public:
	virtual void setup 	(AsyncMqttClient * asyncMqttClient) = 0;
};



