# RelayRemoteControl

This example demonstrates how to control a relay remotely using the ESPWiFiServersExtension.

RelayRemoteControl is an application that uses an ESP8266 Wi-Fi chip to remotely control an electrical relay (an electrically operated switch that opens and closes circuits by receiving electrical signals).

In this example, I used a [Wio node](https://wiki.seeedstudio.com/Wio_Node/) and a [Grove relay](https://wiki.seeedstudio.com/Grove-Relay/) to remotely control my kitchen light :)
There are 2 possibilities in Domoticz to control the relay, using JSON commands or using MQTT commands.

![JSON and MQTT buttons in Domoticz](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/KLightJsonMqtt.png)

## JSON commands

* Turn off the light :  http://[IP address of the Esp8266]/relay/open?json={"id":0}
* Turn on the light  :  http://[IP address of the Esp8266]/relay/close?json={"id":0}

## JSON command in Mozilla Firefox 

![Example in Firefox](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/FirefoxCloseRelay.png)

## JSON commands in Domoticz

![Configuration in Domoticz](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/DomoticzOpenCloseRelay.png)

## MQTT in Domoticz

In Domoticz, I added the MQTT Mosquitto server 

![MQTT mosquitto server settings](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/MqttInDomoticz.png)

Now I can parse the MQTT command coming from a Domoticz button using its idx, see the source files [Settings.h](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/Settings.h) and [MqttRelayDomoticzHandler.cpp](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/MqttRelayDomoticzHandler.cpp)

## Dependencies

To upload this application, you will need the following dependencies:
* [ESPCoreExtension library](https://github.com/gerald-guiony/ESPCoreExtension) library
* [ESPWiFiServersExtension library](https://github.com/gerald-guiony/ESPWiFiServersExtension) library

## Board settings

Board settings in Arduino IDE to upload the sketch to my Wio Node using OTA (Over-The-Air): [Wio Node board settings](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/WioNodeBoardSettings.png)
