//************************************************************************************************************************
// LoggerTelnetServer.cpp
// Version 2.0 June, 2018
// Author Gerald Guiony
//************************************************************************************************************************

#include <map>

#include <Print/LoggerCommandParser.h>

#include "LoggerTelnetServer.h"



using LoggerCommandParserPtr	= std::shared_ptr <LoggerCommandParser>;
using AsyncTCPbufferPtr			= std::shared_ptr <AsyncTCPbuffer>;


struct LoggerTelnetData {
	LoggerCommandParserPtr	parser;
	AsyncTCPbufferPtr		printer;
	FunctionId				fnId;
};


std::map <AsyncClient *, LoggerTelnetData> 	loggerTelnetDataByClientMap;


SINGLETON_IMPL (LoggerTelnetServer)


//========================================================================================================================
//
//========================================================================================================================
void LoggerTelnetServer :: stop () {

	for (auto pair : loggerTelnetDataByClientMap) {
		I(Logger).notifyRequestLineToPrint -= pair.second.fnId;
	}

	loggerTelnetDataByClientMap.clear ();

	TelnetServer :: stop ();
}

//========================================================================================================================
//
//========================================================================================================================
void LoggerTelnetServer :: addClient (AsyncClient * client) {

	LoggerTelnetData & loggerTelnetData = loggerTelnetDataByClientMap [client];

	loggerTelnetData.parser = std::make_shared <LoggerCommandParser> ();
	loggerTelnetData.parser->notifyCloseCurrentSessionResquested	+= [client] ()	{ client->stop (); };
	loggerTelnetData.parser->notifyCloseAllSessionResquested		+= [this] ()	{ stop (); };

	loggerTelnetData.printer = std::make_shared <AsyncTCPbuffer> (client);

	loggerTelnetData.fnId = I(Logger).notifyRequestLineToPrint += [client] (const String & msg) {
		if (loggerTelnetDataByClientMap.find (client) == loggerTelnetDataByClientMap.end()) return;
		loggerTelnetDataByClientMap [client].printer->print (msg);
	};
}

//========================================================================================================================
//
//========================================================================================================================
void LoggerTelnetServer :: deleteClient (AsyncClient * client) {

	if (loggerTelnetDataByClientMap.find (client) == loggerTelnetDataByClientMap.end()) return;
	LoggerTelnetData & loggerTelnetData = loggerTelnetDataByClientMap [client];

	I(Logger).notifyRequestLineToPrint -= loggerTelnetData.fnId;

	loggerTelnetDataByClientMap.erase (client);
}

//========================================================================================================================
//
//========================================================================================================================
void LoggerTelnetServer :: onClientDataReceived (AsyncClient* client, void *data, size_t len) {

	if (loggerTelnetDataByClientMap.find (client) == loggerTelnetDataByClientMap.end()) return;
	LoggerTelnetData & loggerTelnetData = loggerTelnetDataByClientMap [client];

	*loggerTelnetData.printer << F("Data received from client: ") << client->remoteIP() << LN;

	char * byteBuffer = (char *) data;
	for (int i=0; i<len; i++) {
		loggerTelnetData.parser->parse (byteBuffer [i], *loggerTelnetData.printer);
	}
}

//========================================================================================================================
//
//========================================================================================================================
void LoggerTelnetServer :: onClientDisconnected (AsyncClient* client) {

	if (loggerTelnetDataByClientMap.find (client) == loggerTelnetDataByClientMap.end()) return;
	LoggerTelnetData & loggerTelnetData = loggerTelnetDataByClientMap [client];

	*loggerTelnetData.printer << F("Client disconnected !") << LN;

	deleteClient (client);
}

//========================================================================================================================
//
//========================================================================================================================
void LoggerTelnetServer :: onNewClient (void* arg, AsyncClient* client) {

//	client->setNoDelay (true);
	client->setAckTimeout (ASYNC_CLIENT_ACK_TIMEOUT);		// Default is 5000 milliseconds (ASYNC_MAX_ACK_TIME)

	addClient (client);

	TelnetServer :: onNewClient (arg, client);
}

//========================================================================================================================
//
//========================================================================================================================
void LoggerTelnetServer :: setup (uint16_t thePort /*= DEFAULT_TELNET_PORT*/) {

	notifyClientDataReceived += std::bind(&LoggerTelnetServer::onClientDataReceived, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	notifyClientDisconnected += std::bind(&LoggerTelnetServer::onClientDisconnected, this, std::placeholders::_1);

	TelnetServer :: setup (thePort);
}