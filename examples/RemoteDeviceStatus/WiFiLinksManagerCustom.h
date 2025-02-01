//************************************************************************************************************************
// WiFiLinksManagerCustom.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>
#include <WiFiLinksManager.h>


namespace wifix {

//------------------------------------------------------------------------------
// Singleton
class WiFiLinksManagerCustom : public WiFiLinksManager
{
	SINGLETON_CLASS(WiFiLinksManagerCustom)

private:

	bool _isMqttDeviceOnPublished = false;

protected:

	virtual void setupCustomLinks	() override;

public:

	bool isMqttDeviceOnPublished 	() const { return _isMqttDeviceOnPublished; }
};

}