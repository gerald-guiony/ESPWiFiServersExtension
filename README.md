# ESPWiFiServersExtension

<a name="release"></a>
[![Release](https://img.shields.io/github/v/release/gerald-guiony/ESPWiFiServersExtension?include_prereleases)](#release)

[![ESP8266](https://img.shields.io/badge/ESP-8266-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp8266)
[![ESP32](https://img.shields.io/badge/ESP-32-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp32)

ESPWiFiServersExtension is a C++ library for the Arduino Core designed for ESP8266 and ESP32 Wi-Fi chips. It extends the functionalities of the [ESP Async WebServer](https://github.com/me-no-dev/ESPAsyncWebServer) and [Async MQTT Client libraries](https://github.com/marvinroger/async-mqtt-client)

This library, compatible with ESP32 and ESP8266 cards, allows you to quickly develop applications embedding an asynchronous HTTP server and/or an asynchronous Domoticz MQTT client.

## Getting started

To use ESPWiFiServersExtension, you will need:

* An ESP8266 or ESP32 board
* The Arduino IDE configured for ESP8266 or ESP32 (version 2.3.2 or later)
* A basic understanding of the Arduino environment (e.g., uploading sketches, importing libraries, etc.)

## Installing ESPWiFiServersExtension

1. Download the latest master source code as a [.zip](https://github.com/gerald-guiony/ESPWiFiServersExtension/archive/master.zip) file
2. In the Arduino IDE, import the `.zip` file by navigating to **Sketch → Include Library → Add .ZIP Library**

## Dependencies

To compile this library, you must have: 

* Latest version of my [ESPCoreExtension](https://github.com/gerald-guiony/ESPCoreExtension) library
* Latest version of my forked version of [AsyncTCP](https://github.com/gerald-guiony/AsyncTCP) library
* Version 1.2.4 of [ESP Async WebServer](https://github.com/me-no-dev/ESPAsyncWebServer) library
* Version 0.9.0 of [AsyncMqttClient](https://github.com/marvinroger/async-mqtt-client) library
* Version 5.13.4 of [ArduinoJson](https://github.com/bblanchon/ArduinoJson) library
* Version 2.0.17 of [WiFiManager](https://github.com/tzapu/WiFiManager) library

## Debug all your sketches remotely with Telnet

This library allows you to perform remote debugging via Telnet in a basic terminal:
> telnet [IP address of your Esp]

![Settings in Domoticz](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/doc/debug.png)

Debugging commands are also available; type 'h' to display the list of commands:

![Settings in Domoticz](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/doc/debugCommand.png)

## Administrative JSON commands

In this library, you can access JSON administrative commands for your ESP. Refer to this source file for more details:  

[HttpAdminCommandRequestHandler.cpp](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/src/HttpAdminCommandRequestHandler.cpp)

> http:://[IP address of your Esp]/admin/help

![Admin commands](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/doc/JsonAdminCommand.png)

## Examples

1. Remotely control an electrical relay: [RelayRemoteControl](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl)
2. Device waking up from standby mode: [RemoteDeviceStatus](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RemoteDeviceStatus)
3. Universal InfraRed remote control: [IrRemoteControl](https://github.com/gerald-guiony/ESPInfraredTransceiver/blob/master/examples/IrRemoteControl)
