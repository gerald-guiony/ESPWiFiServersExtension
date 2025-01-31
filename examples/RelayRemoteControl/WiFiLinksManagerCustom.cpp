//************************************************************************************************************************
// WiFiLinksManagerCustom.cpp
// Version 1.0 August, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#include <HttpServer.h>
#include <MqttDomoticzClient.h>
#include <MqttAWSIoTCoreClient.h>

#include "Settings.h"
#include "secrets.h"

#include <HttpAdminCommandRequestHandler.h>

#include "HttpRelayCommandRequestHandlerWiFi.h"
#include "MqttRelayDomoticzHandler.h"

#include "WiFiLinksManagerCustom.h"


#ifdef USING_WIFI
#	warning -- USING_WIFI defined --
#endif
#ifdef USING_DOMOTICZ_MQTT
#	warning -- USING_DOMOTICZ_MQTT defined --
#endif
#ifdef USING_AWS_MQTT
#	warning -- USING_AWS_MQTT defined --
#endif


namespace wifix {

SINGLETON_IMPL (WiFiLinksManagerCustom)

//========================================================================================================================
//
//========================================================================================================================
void WiFiLinksManagerCustom :: setupCustomLinks ()
{
	// Setup the Web Servers
	I(HttpServer).setup ({ &I(HttpAdminCommandRequestHandler), &I(HttpRelayCommandRequestHandler) });
	addCustomLinks ({&I(HttpServer)});

#ifdef USING_DOMOTICZ_MQTT
	// Setup Domoticz MQTT Client
	I(MqttDomoticzClient).setup (MQTT_DOMOTICZ_ENDPOINT, MQTT_DOMOTICZ_PORT, {	&I(MqttDomoticzLogPublisher),
																	 			&I(MqttRelayDomoticzPublisher),
																				&I(MqttRelayDomoticzSubscriber) });
	addCustomLinks ({&I(MqttDomoticzClient)});
#endif

#ifdef USING_AWS_MQTT
	// Setup AWS Iot Core MQTT client
	I(MqttAWSIoTCoreClient).setup ( AWS_IOT_ENDPOINT, AWS_IOT_PORT, THINGNAME,
									AWS_CERT_CA, AWS_CERT_CRT, AWS_CERT_PRIVATE,
									{});

	addCustomLinks ({&I(MqttAWSIoTCoreClient)});
#endif
}

}
