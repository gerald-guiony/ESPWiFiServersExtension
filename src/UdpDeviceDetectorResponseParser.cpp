//************************************************************************************************************************
// UdpDeviceDetectorResponseParser.cpp
// Version 1.0 June, 2017
// Author Gerald Guiony
//************************************************************************************************************************

#include "UdpDeviceDetectorResponseParser.h"


namespace wifix {


SINGLETON_IMPL (UdpDeviceDetectorResponseParser)


//========================================================================================================================
//
//========================================================================================================================
bool UdpDeviceDetectorResponseParser :: parse (Stream & stream, Print & printer) {

	while (stream.available()) {
		if (checkRespBegin (stream)) break;
	}

	if (!stream.available()) return false;

	Logln (LN);

	int respId = getRespId (stream);
	if (respId < 0) {
		Logln(F("Invalid response id < 0 ?"));
		return false;
	}

	switch (respId)
	{
	case 0:
		Logln(F("=> HELP"));
		break;

	case 1:
		{
			Logln(F("=> NAME, CATEGORY, IP AND MAC ADDRESS"));

			if (!checkSeparatorRespParam (stream)) {
				Logln(F("Separator response param invalid !"));
				return false;
			}

			Logln(F("Params received:"));
			String param;
			do {
				param = getRespParam (stream);
				printer << param << LN;
				Logln(param);
			}
			while (checkSeparatorParam (stream));

//			if (!checkRespEnd (stream)) {
//				Logln(F("Response end not found !"));
//				return false;
//			}
		}
		break;

	default:
		Logln(F("Invalid response id"));
		return false;
	}

	return true;
}

}










