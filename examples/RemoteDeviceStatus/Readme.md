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

When the Freebox Player exits standby, it powers its USB ports and, consequently, the Wio Node :sunglasses: 
The Wio Node then notifies its ON status through MQTT every 5 minutes as long as it remains powered.
For energy saving reasons, each time the Wio Node has published its status it enters deep sleep mode for 5 minutes :zzz:

see the source files for more details.

## Domoticz and MQTT

In Domoticz, with the help of MQTT mosquitto server I can show the status of my Freebox Player coming from MQTT using a simple Domoticz On/Off button. 

![Simple Domoticz button](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus/doc/img/SimpleDomoticzButton.png)

Additionally, a Lua script in Domoticz checks how long the Freebox Player On/Off button has not been updated, if it has been more than 6 minutes its status is set to Off.

## Dependencies

To upload this application you must have the latest version of:
* [ESPCoreExtension library](https://github.com/gerald-guiony/ESPCoreExtension)
* [ESPWiFiServersExtension library](https://github.com/gerald-guiony/ESPWiFiServersExtension)

## Board settings

Here are the board settings in the Arduino IDE that I use to upload the sketch to my Wio Node via OTA (Over-The-Air): [Wio Node board settings](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus/doc/WioNodeBoardSettings.png)
