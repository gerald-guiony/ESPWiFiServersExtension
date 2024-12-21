This sketch has been developed for the version 3.1.2 of the Arduino Core for ESP8266 WiFi chip. Switch to that in the Board Manager, if you havn't done already.

# RemoteDeviceStatus

RemoteDeviceStatus is an application that uses an ESP8266 WiFi chip to indicate whether a device has power.
Particularly useful if the device has its USB ports powered when waking up from standby mode.

Here I used a [Wio node](https://wiki.seeedstudio.com/Wio_Node/) to know if my [Freebox Player](https://www.actusfree.fr/freebox-player/) has come out of standby mode.

## Freebox Player settings 

![Freebox settings](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus/doc/img/FreeboxPlayerUsbDisabled.jpg)

## Freebox Player with an esp8266 Wio Node on its USB port 

![Freebox Wio Node](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus/doc/img/FreeboxPlayerWioNode.jpg)

## Freebox Player Status

When the Freebox Player comes out of standby it powers its USB ports and therefore the Wio Node and this one notify its ON status through MQTT every 5 minutes as long as it is powered.
see the source files [Settings.h](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus/Settings.h) and [MqttRelayDomoticzHandler.cpp](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus/MqttDeviceONDomoticzHandler.cpp)

Additionally, a Lua script in Domoticz pings the Wio node to update its status to Off when it is no longer powered.

## Domoticz and MQTT

Now in Domoticz, with the help of MQTT mosquitto server I can show the status of my Freebox Player coming from MQTT or updated by a Lua script with a simple Domoticz On/Off button. 

## Simple On/Off Domoticz button

![Simple Domoticz button](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus/doc/img/SimpleDomoticzButton.png)
