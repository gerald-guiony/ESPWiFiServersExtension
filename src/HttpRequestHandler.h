//************************************************************************************************************************
// HttpRequestHandler.h
// Version 1.0 August, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#pragma once

#include <ESPAsyncWebServer.h>

#include <Module/AsyncModule.h>

using namespace corex;


namespace wifix {

//------------------------------------------------------------------------------
//
class HttpRequestHandler : public AsyncModule <AsyncWebServer &>
{
};

}



