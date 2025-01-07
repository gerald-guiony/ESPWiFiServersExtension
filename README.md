# ESPWiFiServersExtension

<a name="release"></a>
[![Release](https://img.shields.io/github/v/release/gerald-guiony/ESPWiFiServersExtension?include_prereleases)](#release)

[![ESP8266](https://img.shields.io/badge/ESP-8266-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp8266)
[![ESP32](https://img.shields.io/badge/ESP-32-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp32)

ESPWiFiServersExtension is a C++ library for the Arduino Core for ESP8266 & ESP32 WiFi chip which extends the libraries [ESP async Webserver](https://github.com/me-no-dev/ESPAsyncWebServer) and [async Mqtt Client](https://github.com/marvinroger/async-mqtt-client)

## Getting started

To use ESPWiFiServersExtension, you need:

* An ESP8266 or ESP32
* The Arduino IDE for ESP8266 or ESP32 (version 2.3.2 minimum)
* Basic knowledge of the Arduino environment (upload a sketch, import libraries, ...)

## Installing ESPWiFiServersExtension

1. Download the latest master source code [.zip](https://github.com/gerald-guiony/ESPWiFiServersExtension/archive/master.zip)
2. In the Arduino IDE, load the `.zip` with **Sketch → Include Library → Add .ZIP Library**

## Dependencies

To compile this library you must have: 

* Latest version of [ESPCoreExtension](https://github.com/gerald-guiony/ESPCoreExtension)
* Version 5.13.4 of [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* Version 2.0.17 of [WiFiManager](https://github.com/tzapu/WiFiManager)
* Version 1.2.4 of [ESP Async WebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
* Version 0.9.0 of [AsyncMqttClient](https://github.com/marvinroger/async-mqtt-client)

## Debug all your sketches remotely with Telnet

With this library you can debug remotely with telnet in a terminal :
> telnet [IP address of your Esp]

![Settings in Domoticz](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/doc/debug.png)

You also have debugging commands, type 'h' to see the list of these commands :

![Settings in Domoticz](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/doc/debugCommand.png)

## Administrative JSON commands

In this library you can access to administrative commands for your esp with JSON, see source file  

[HttpAdminCommandRequestHandler.cpp](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/src/HttpAdminCommandRequestHandler.cpp)

> http:://[IP address of your Esp]/admin/help

![Admin commands](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/doc/JsonAdminCommand.png)

## Examples

1. Control remotely an electrical relay : [RelayRemoteControl](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl)
2. Device waking up from standby mode : [RemoteDeviceStatus](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus)
3. Universal InfraRed remote control : [IrRemoteControl](https://github.com/gerald-guiony/ESPInfraredTransceiver/blob/master/examples/IrRemoteControl)
