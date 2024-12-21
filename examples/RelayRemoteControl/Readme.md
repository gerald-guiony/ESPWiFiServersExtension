This sketch has been developed for the version 3.1.2 of the Arduino Core for ESP8266 WiFi chip. Switch to that in the Board Manager, if you havn't done already.

# RelayRemoteControl

RelayRemoteControl is an application which uses an ESP8266 WiFi chip to control remotely an electrical relay (electrically operated switches that open and close the circuits by receiving electrical signals)

Here I used a [Wio node](https://wiki.seeedstudio.com/Wio_Node/) and this [Grove relay](https://wiki.seeedstudio.com/Grove-Relay/) to remotely control my kitchen light :)

## JSON commands

* Turn off the light :  http://[IP address of the Esp8266]/relay/open?json={"id":0}
* Turn on the light  :  http://[IP address of the Esp8266]/relay/close?json={"id":0}

## JSON command in Mozilla Firefox 

![Sample in Firefox](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/FirefoxCloseRelay.png)

## JSON commands in Domoticz

![Settings in Domoticz](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/DomoticzOpenCloseRelay.png)

## MQTT

In Domoticz, I added the MQTT mosquitto server 

![MQTT mosquitto server settings](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/doc/img/MqttInDomoticz.png)

Now I can parse the mqtt command coming from a Domoticz button using its name, see the source files [Settings.h](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/Settings.h) and [MqttRelayDomoticzHandler.cpp](https://github.com/gerald-guiony/ESPWiFiServersExtension/blob/master/examples/RelayRemoteControl/MqttRelayDomoticzHandler.cpp)

