# RemoteDeviceStatus

RemoteDeviceStatus is an application that uses an ESP8266 Wi-Fi chip to indicate whether a device has power,
particularly useful if the device powers its USB ports when waking up from standby mode.

In this example, I used a [Wio node](https://wiki.seeedstudio.com/Wio_Node/) to determine if my [Freebox Player](https://www.actusfree.fr/freebox-player/) has come out of standby mode. 
However, you can apply this principle to any other device that powers its USB ports when exiting standby mode.

This approach eliminates the need to install a Domoticz plugin for the Freebox, such as [this one](https://github.com/supermat/PluginDomoticzFreebox) which requires the registration of 
a connection token and modification rights.

## Freebox Player settings 

![Freebox settings](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus/doc/img/FreeboxPlayerUsbDisabled.jpg)

## Freebox Player with an esp8266 Wio Node on its USB port 

![Freebox with a Wio Node](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus/doc/img/FreeboxPlayerWioNode.jpg)

## Freebox Player Status

When the Freebox Player exits standby, it powers its USB ports and, consequently, the Wio Node. The Wio Node then notifies its ON status through MQTT every 5 minutes as long as it remains powered..
see the source files [Settings.h](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus/Settings.h) and [MqttRelayDomoticzHandler.cpp](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus/MqttDeviceONDomoticzHandler.cpp)
for more details.

Additionally, a Lua script in Domoticz pings the Wio node to update its status to Off when it is no longer powered.

## Domoticz and MQTT

In Domoticz, with the help of MQTT mosquitto server I can show the status of my Freebox Player coming from MQTT or updated by a Lua script with a simple Domoticz On/Off button. 

![Simple Domoticz button](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus/doc/img/SimpleDomoticzButton.png)

## Dependencies

To upload this application you must have the latest version of:
* [ESPCoreExtension](https://github.com/gerald-guiony/ESPCoreExtension) library 
* [ESPWiFiServersExtension](https://github.com/gerald-guiony/ESPWiFiServersExtension) library

## Board settings

Board settings in Arduino IDE to upload the sketch to my Wio Node using OTA (Over-The-Air): [Wio Node board settings](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus/doc/WioNodeBoardSettings.png)
