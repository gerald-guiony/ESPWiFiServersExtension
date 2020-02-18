//************************************************************************************************************************
// DeviceDetectorResponseParser.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>

//------------------------------------------------------------------------------
// WARNING : SINGLETON !!!!
class DeviceDetectorResponseParser : public StreamRespParser
{
	SINGLETON_CLASS(DeviceDetectorResponseParser)

public:
	virtual bool parse				(Stream & stream, Print & printer) override;
};