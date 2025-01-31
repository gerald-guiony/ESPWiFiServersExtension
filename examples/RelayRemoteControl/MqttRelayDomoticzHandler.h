//************************************************************************************************************************
// MqttRelayDomoticzHandler.h
// Version 1.0 February, 2020
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>
#include <MqttDomoticzHandler.h>


namespace wifix {

//------------------------------------------------------------------------------
// Singleton
class MqttRelayDomoticzPublisher : public MqttDomoticzPublisher
{
	SINGLETON_CLASS(MqttRelayDomoticzPublisher)

public:
	size_t publishStatutMessage		(const String & msg);
};


//------------------------------------------------------------------------------
// Singleton
class MqttRelayDomoticzSubscriber : public MqttDomoticzSubscriberIdx
{
	SINGLETON_CLASS(MqttRelayDomoticzSubscriber)

public:

	virtual void setup 					(AsyncMqttClient * asyncMqttClient) override;
	virtual bool onMqttMsgReceivedIdx	(const JsonObject& jsonObj) override;
};


}






