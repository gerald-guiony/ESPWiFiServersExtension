//************************************************************************************************************************
// UdpDeviceDetectorResponseParser.h
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <Common.h>

using namespace corex;

namespace wifix {

//------------------------------------------------------------------------------
// WARNING : SINGLETON !!!!
class UdpDeviceDetectorResponseParser : public StreamRespParser
{
	SINGLETON_CLASS(UdpDeviceDetectorResponseParser)

public:
	virtual bool parse	(Stream & stream, Print & printer) override;
};

}