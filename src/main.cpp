/*
 * Smart Radio - A ESP8266 and VS1053 driven WebRadio and Internet music player.
 *
 * Creator: Henry Li
 */

// Libraries for Arduino
#include <Arduino.h>
#include <SPI.h>

// Libraries for ESP8266 - Arduino
#include <ArduinoOTA.h>
#include <FS.h>

// Libraries for Hardwares
#include <VS1053.h>

// Libraries by me
#include <Console.h>
#include <URLParser.h>

#include "./stream/GeneralInputBuffer.h"
#include "./media/MediaOutputBuffer.h"
#include "./media/MediaPlayer.h"

extern "C" {
#include "user_interface.h"
}

// VS1053 Settings
#define VS1053_CS_PIN D1
#define VS1053_DCS_PIN D0
#define VS1053_DREQ_PIN D2

// Global Variables

// Global Objects
VS1053 vs1053(VS1053_CS_PIN, VS1053_DCS_PIN, VS1053_DREQ_PIN);
MediaPlayer mediaPlayer(&vs1053);

// Function Declarations
bool setupWiFi();
bool playLocalFile(String path);
bool playRemoteUrl(String url);


void setup()
{
    Console::begin();
    Console::line();
    Console::info("SmartRadio is now starting...");

    // Set to 160 MHz in order to get better I/O performance
    // With ESP8266 running at 80 MHz, it is capable of handling up to 256 kb bitrate.
    // With ESP8266 running at 160 MHz, it is capable of handling up to 320 kb bitrate.
    Console::info("Setting CPU frequency to 160Mhz...");
    system_update_cpu_freq(85);

    // Here we use SPIFFS(ESP8266 built-in File System) to store stations and other settings,
    // as well as short sound effects.
    Console::info("Setting up file system....");
    SPIFFS.begin();

    Console::info("Setting up WiFi...");
    bool wifiConnected = setupWiFi();
    if (!wifiConnected)
    {
        Console::fatal("SmartRadio failed to start up.\nReason: No WiFi Connection.");
        return;
    }

    // Setup OTA firmware update
    Console::info("Setting up OTA...");
    ArduinoOTA.begin();

    // Setup VS1053
    SPI.begin();
    bool mediaPlayerEnabled = mediaPlayer.begin();
    if (!mediaPlayerEnabled)
    {
        Console::fatal("SmartRadio failed to start up.\nReason: VS1053 can not be started.");
        return;
    }

    Console::info("SmartRadio is now running...");

    // mediaPlayer.loadLocalFile("/test.mp3");
    mediaPlayer.loadRemoteURL("http://lhttp.qingting.fm/live/387/64k.mp3");
    // mediaPlayer.loadRemoteURL("http://m2.music.126.net/NG4I9FVAm9jCQCvszfLB8Q==/1377688074172063.mp3");
}

void loop()
{
    ArduinoOTA.handle();
    mediaPlayer.handle();
}


bool setupWiFi()
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
