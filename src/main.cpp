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
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <FS.h>

// Libraries for Hardwares
#include <VS1053.h>

// Libraries by me
#include <Console.h>
#include <URLParser.h>

#include "./media/MediaOutputBuffer.h"
#include "./stream/GeneralInputBuffer.h"

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
WiFiClient httpClient;

// currentInputStream could be pointed to a FileInputStream or NetworkInputStream
Stream *currentInputStream = NULL;
File fileInputStream; // Input stream for local files.

// Buffer for input stream from file or network
GeneralInputBuffer generalInputBuffer;
// Buffer for output stream to VS1053
MediaOutputBuffer mediaOutputBuffer(&vs1053);

// Function Declarations
bool setupWiFi();
bool playLocalFile(String path);
bool playRemoteUrl(String url);


void setup()
{
    Console::begin();
    Console::line();
    Console::info("smart-radio is now starting...");

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
        Console::error("smart-radio failed to start up.");
        return;
    }

    Console::info("Setting up OTA...");
    ArduinoOTA.begin();

    // Setup VS1053
    SPI.begin();
    bool vs1053Enabled = vs1053.begin();
    if (!vs1053Enabled)
    {
        Console::error("smart-radio failed to start up.");
        return;
    }
    // Set the initial volume
    vs1053.setVolume(70);

    Console::info("smart-radio is now running...");

    // playLocalFile("/test.mp3");
    playRemoteUrl("http://lhttp.qingting.fm/live/387/64k.mp3");
    // playRemoteUrl("http://m2.music.126.net/NG4I9FVAm9jCQCvszfLB8Q==/1377688074172063.mp3");
}

void loop()
{
    // Handling OTA request
    ArduinoOTA.handle();

    // ****** READING FROM INPUT STREAM *******
    // When currentInputStream has been assigned
    if (currentInputStream)
    {
        generalInputBuffer.readFromStream(currentInputStream);
    }

    // ********* KEEP VS1053 FILLED **********
    while (vs1053.data_request() && generalInputBuffer.available())
    {
        mediaOutputBuffer.write(generalInputBuffer.read());
    }
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

bool playLocalFile(String path)
{
    Console::info("Loading %s...", path.c_str());
    fileInputStream = SPIFFS.open(path, "r"); // Open the file
    if (!fileInputStream)
    {
        Console::info("Error opening file %s", path.c_str()); // No luck
        return false;
    }
    currentInputStream = &fileInputStream;
    Console::info("%s has been loaded.", path.c_str());
    return true;
}

bool playRemoteUrl(String urlString)
{
    Console::info("Loading %s...", urlString.c_str());
    URL url = parseURL(urlString);
    if (url.isValid)
    {
        if (url.protocol.equals("http"))
        {
            if (httpClient.connect(url.host.c_str(), url.port))
            {
                httpClient.print(String("GET ") + url.path + " HTTP/1.1\r\n" + "Host: " + url.host +
                                 "\r\n" + "Connection: close\r\n\r\n");
                currentInputStream = &httpClient;
                Console::info("%s has been loaded.", urlString.c_str());
                return true;
            }
        }
        else
        {
            Console::error("Invalid protocol. Currently we only support HTTP protocol.");
        }
    }
    else
    {
        Console::error("Invalid URL.");
    }
    return false;
}
