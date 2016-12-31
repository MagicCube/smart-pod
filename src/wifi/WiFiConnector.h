#ifndef WIFI_CONNECTOR_H
#define WIFI_CONNECTOR_H

#include <Arduino.h>

#include <ESP8266WiFi.h>

class WiFiConnector
{
  public:
    static bool begin();
};

#endif
