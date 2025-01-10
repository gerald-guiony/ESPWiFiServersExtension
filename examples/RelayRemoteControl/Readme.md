# RelayRemoteControl

RelayRemoteControl is an application which uses an ESP8266 WiFi chip to control remotely an electrical relay (electrically operated switches that open and close the circuits by receiving electrical signals)

Here I used a [Wio node](https://wiki.seeedstudio.com/Wio_Node/) and this [Grove relay](https://wiki.seeedstudio.com/Grove-Relay/) to remotely control my kitchen light :)
In this example there are 2 possibilities in Domoticz to control the relay, using JSON commands or using MQTT commands.

![JSON and MQTT buttons in Domoticz](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/KLightJsonMqtt.png)

## JSON commands

* Turn off the light :  http://[IP address of the Esp8266]/relay/open?json={"id":0}
* Turn on the light  :  http://[IP address of the Esp8266]/relay/close?json={"id":0}

## JSON command in Mozilla Firefox 

![Sample in Firefox](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/FirefoxCloseRelay.png)

## JSON commands in Domoticz

![Settings in Domoticz](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/DomoticzOpenCloseRelay.png)

## MQTT in Domoticz

In Domoticz, I added the MQTT mosquitto server 

![MQTT mosquitto server settings](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/MqttInDomoticz.png)

Now I can parse the mqtt command coming from a Domoticz button using its idx, see the source files [Settings.h](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/Settings.h) and [MqttRelayDomoticzHandler.cpp](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/MqttRelayDomoticzHandler.cpp)

## Dependencies

To upload this application you must have the latest version of :
* [ESPCoreExtension library](https://github.com/gerald-guiony/ESPCoreExtension) 
* [ESPWiFiServersExtension library](https://github.com/gerald-guiony/ESPWiFiServersExtension)

## Board settings

Board settings in Arduino IDE to upload the sketch with OTA (Over The Air) to my Wio node : [Wio Node board settings](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/WioNodeBoardSettings.png)
