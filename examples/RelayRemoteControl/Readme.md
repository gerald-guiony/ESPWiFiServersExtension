# RelayRemoteControl

This example demonstrates how to control a relay remotely using the ESPWiFiServersExtension.

RelayRemoteControl is an application that uses an ESP8266 Wi-Fi chip to remotely control an electrical relay (an electrically operated switch that opens and closes circuits by receiving electrical signals).

In this example, I used a [Wio node](https://wiki.seeedstudio.com/Wio_Node/) and a [Grove relay](https://wiki.seeedstudio.com/Grove-Relay/) to remotely control my kitchen light. 
I added 2 switches in Domoticz to control the relay, using JSON request or MQTT topic.

![JSON and MQTT buttons in Domoticz](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/KLightJsonMqtt.png)

I also use it using an [ESP32 WT32-ETH01 Ethernet Module](https://werner.rothschopf.net/microcontroller/202401_esp32_wt32_eth01_en.htm) to power my pool pump (WiFi not accessible)

The purpose of this Ethernet smart plug is to operate the filtration of my swimming pool depending on the weather conditions and also depending on the cost of electricity at 
certain times of the day ("Heures creuses"). For this I wrote a lua script in Domoticz.

![WT32 ETH01 smart plug](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/WT32ETHSmartPlug.jpg)
![WT32 ETH01 smart plug plugged in](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/WT32ETHSmartPlugIn.jpg)

## JSON requests

* Turn off the light :  http://[IP address of the Esp8266]/relay/open
* Turn on the light  :  http://[IP address of the Esp8266]/relay/close

## JSON request in Mozilla Firefox 

![Example in Firefox](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/FirefoxCloseRelay.png)

## JSON request parameters in Domoticz

![Configuration in Domoticz](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/DomoticzOpenCloseRelay.png)

## MQTT in Domoticz

In Domoticz, I added the MQTT Mosquitto server 

![MQTT mosquitto server settings](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/MqttInDomoticz.png)

Now I can parse the MQTT command coming from a Domoticz switch using its idx, see the source files [Settings.h](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/Settings.h) and [MqttRelayDomoticzHandler.cpp](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/MqttRelayDomoticzHandler.cpp)

## Prerequisites

To upload this application, you will need the following dependencies:
* [ESPCoreExtension library](https://github.com/gerald-guiony/ESPCoreExtension)
* [ESPWiFiServersExtension library](https://github.com/gerald-guiony/ESPWiFiServersExtension)

For the ESP32 WT32-ETH01 you will also need:
* [AsyncWebServer_WT32_ETH01 library](https://github.com/gerald-guiony/AsyncWebServer_WT32_ETH01)

## Board settings

Here are the board settings in the Arduino IDE that I use to upload the sketch via OTA (Over-The-Air):

* [Wio Node board settings](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/WioNodeBoardSettings.png)
* [WT32-ETH01 board settings](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/WT32ETH01BoardSettings.png)
