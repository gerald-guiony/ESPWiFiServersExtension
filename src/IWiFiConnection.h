//************************************************************************************************************************
// IWiFiConnection.h
// Version 1.0 jan, 2025
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

namespace wifix {

class IWiFiConnection
{
public:
    virtual ~IWiFiConnection() = default;

	virtual	void connect	() = 0;
	virtual void disconnect () = 0;

	// AsyncModule or other by default ...
	virtual bool isModule () const { return false; }
};

}