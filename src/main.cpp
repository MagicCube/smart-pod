/*
 * Smart Radio - A ESP8266 and VS1053 driven WebRadio and Internet music player.
 *
 * Creator: Henry Li
 */

#include <Arduino.h>
#include <FS.h>
#include <SPI.h>

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include <VS1053.h>
#include <log.h>

#include "./stream/MediaInputBuffer.h"
#include "./stream/MediaOutputBuffer.h"
#include "./util/URLParser.h"

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

// Buffer for input stream from file or network
MediaInputBuffer mediaInputBuffer;
// Buffer for output stream to VS1053
MediaOutputBuffer mediaOutputBuffer(&vs1053);

// mediaInputStream could be pointed to a FileInputStream or NetworkInputStream
Stream *mediaInputStream = NULL;
File fileInputStream; // Input stream for local files.


// Function Declarations
bool setupWiFi();
bool playLocalFile(String path);
bool playRemoteUrl(String url);


void setup()
{
    Serial.begin(115200);

    Serial.println("\n\n");
    log("smart-radio is now starting...");

    // Set to 160 MHz in order to get better I/O performance
    // With ESP8266 running at 80 MHz, it is capable of handling up to 256 kb bitrate.
    // With ESP8266 running at 160 MHz, it is capable of handling up to 320 kb bitrate.
    log("Setting CPU frequency to 160Mhz...");
    system_update_cpu_freq(80);

    // Here we use SPIFFS(ESP8266 built-in File System) to store stations and other settings,
    // as well as short sound effects.
    log("Setting up file system....");
    SPIFFS.begin();

    log("Setting up WiFi...");
    setupWiFi();
    // log("Setting up OTA...");
    // ArduinoOTA.begin();

    // Setup VS1053
    SPI.begin();
    vs1053.begin();
    // Set the initial volume
    vs1053.setVolume(70);

    log("smart-radio is now running...");

    // playLocalFile("/test.mp3");
    playRemoteUrl("http://lhttp.qingting.fm/live/387/64k.mp3");
    //playRemoteUrl("http://m2.music.126.net/NG4I9FVAm9jCQCvszfLB8Q==/1377688074172063.mp3");
}

void loop()
{
    // Handling OTA request
    // ArduinoOTA.handle();

    // ****** READING FROM INPUT STREAM *******
    // When mediaInputStream has been assigned
    if (mediaInputStream)
    {
        mediaInputBuffer.readFromStream(mediaInputStream);
    }

    // ********* KEEP VS1053 FILLED **********
    while (vs1053.data_request() && mediaInputBuffer.available())
    {
        mediaOutputBuffer.write(mediaInputBuffer.read());
    }
}


bool setupWiFi()
{
    WiFi.mode(WIFI_STA);
    log("Scanning WiFi...");
    int ssidCount = WiFi.scanNetworks();
    if (ssidCount == -1)
    {
        log("Couldn't get a WiFi connection.");
        return false;
    }

    String WIFI_SSID = "none";
    String WIFI_PWD;
    for (int i = 0; i < ssidCount; i++)
    {
        String ssid = WiFi.SSID(i);
        if (ssid.equals("Henry's Living Room 2.4GHz"))
        {
            WIFI_SSID = ssid;
            WIFI_PWD = "13913954971";
            break;
        }
        else if (ssid.equals("Henry's TP-LINK"))
        {
            WIFI_SSID = ssid;
            WIFI_PWD = "13913954971";
            break;
        }
        else if (ssid.equals("Henry's iPhone 6"))
        {
            WIFI_SSID = ssid;
            WIFI_PWD = "13913954971";
            // Don't break, cause this will connect to 4G network.
            // It's absolutely not a first choise.
        }
    }
    if (WIFI_SSID.equals("none"))
    {
        log("Couldn't find a recognized WiFi connection.");
        return false;
    }

    WiFi.begin(WIFI_SSID.c_str(), WIFI_PWD.c_str());
    Serial.print("<i> Connecting to ");
    Serial.print(WIFI_SSID);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    log("Got IP: %s", WiFi.localIP().toString().c_str());
    return true;
}

bool playLocalFile(String path)
{
    log("Loading %s...", path.c_str());
    fileInputStream = SPIFFS.open(path, "r"); // Open the file
    if (!fileInputStream)
    {
        log("Error opening file %s", path.c_str()); // No luck
        return false;
    }
    mediaInputStream = &fileInputStream;
    log("%s has been loaded.", path.c_str());
    return true;
}

bool playRemoteUrl(String urlString)
{
    log("Loading %s...", urlString.c_str());
    URL url = parseURL(urlString);
    if (url.isValid)
    {
        if (httpClient.connect(url.host.c_str(), url.port))
        {
            httpClient.print(String("GET ") + url.path + " HTTP/1.1\r\n" + "Host: " + url.host +
                             "\r\n" + "Connection: close\r\n\r\n");
            mediaInputStream = &httpClient;
            log("%s has been loaded.", urlString.c_str());
            return true;
        }
    }
    else
    {
        log("Invalid URL.");
    }
    return false;
}
