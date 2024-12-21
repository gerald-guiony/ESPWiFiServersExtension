//************************************************************************************************************************
// MqttDeviceONDomoticzHandler.h
// Version 1.0 February, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>
#include <MqttDomoticzHandler.h>


//------------------------------------------------------------------------------
// Singleton
class MqttDeviceONDomoticzPublisher : public MqttDomoticzPublisher
{
	SINGLETON_CLASS(MqttDeviceONDomoticzPublisher)

public:
	size_t publishDeviceON 				();
};


//------------------------------------------------------------------------------
// Singleton
class MqttDeviceONDomoticzSubscriber : public MqttDomoticzSubscriber
{
	SINGLETON_CLASS(MqttDeviceONDomoticzSubscriber)

public:
	virtual bool parseJsonObj			(const JsonObject& jsonObj) override;
};










