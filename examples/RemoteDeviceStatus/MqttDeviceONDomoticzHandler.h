//************************************************************************************************************************
// MqttDeviceONDomoticzHandler.h
// Version 1.0 February, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>
#include <MqttDomoticzHandler.h>

namespace wifix {

//------------------------------------------------------------------------------
// Singleton
class MqttDeviceONDomoticzPublisher : public MqttDomoticzPublisher
{
	SINGLETON_CLASS(MqttDeviceONDomoticzPublisher)

private:
	bool _isDeviceOnPublished = false;

public:
	size_t publishDeviceONState 	();
	bool deviceONStatePublished 	() const { return _isDeviceOnPublished; }
};


//------------------------------------------------------------------------------
// Singleton
class MqttDeviceONDomoticzSubscriber : public MqttDomoticzSubscriberIdx
{
	SINGLETON_CLASS(MqttDeviceONDomoticzSubscriber)

public:
	virtual void setup 				(AsyncMqttClient * asyncMqttClient) override;
	virtual bool onTopicIdxReceived	(const JsonObject& jsonObj) override;
};

}








