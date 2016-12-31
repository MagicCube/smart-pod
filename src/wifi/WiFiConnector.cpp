#include <Console.h>

#include "WiFiConnector.h"

bool WiFiConnector::begin()
{
    WiFi.mode(WIFI_STA);

    // Auto scan WiFi connection
    String prefSSID = "none";
    String prefPassword;
    Console::info("Scanning WiFi...");
    int ssidCount = WiFi.scanNetworks();
    if (ssidCount == -1)
    {
        Console::error("Couldn't get a WiFi connection.");
        return false;
    }
    for (int i = 0; i < ssidCount; i++)
    {
        String ssid = WiFi.SSID(i);
        if (ssid.equals("Henry's Living Room 2.4GHz"))
        {
            prefSSID = ssid;
            prefPassword = "13913954971";
            break;
        }
        else if (ssid.equals("Henry's TP-LINK"))
        {
            prefSSID = ssid;
            prefPassword = "13913954971";
            break;
        }
        else if (ssid.equals("Henry's iPhone 6"))
        {
            prefSSID = ssid;
            prefPassword = "13913954971";
            // Don't break, cause this will connect to 4G network.
            // It's absolutely not a first choise.
        }
    }
    if (prefSSID.equals("none"))
    {
        Console::error("Couldn't find a recognized WiFi connection.");
        return false;
    }
    WiFi.begin(prefSSID.c_str(), prefPassword.c_str());
    Console::info("Connecting to \"%s\"", prefSSID.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Console::info("Got IP: %s", WiFi.localIP().toString().c_str());
    return true;
}
