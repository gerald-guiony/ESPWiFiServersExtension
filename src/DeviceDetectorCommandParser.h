//************************************************************************************************************************
// DeviceDetectorCommandParser.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>

//------------------------------------------------------------------------------
// WARNING : SINGLETON !!!!
class DeviceDetectorCommandParser : public StreamCmdParser
{
	SINGLETON_CLASS(DeviceDetectorCommandParser)

public:
	virtual bool parse				(Stream & stream, Print & printer) override;
};
