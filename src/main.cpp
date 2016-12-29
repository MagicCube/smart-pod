/*
 * Smart Radio
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

#include "MediaInputBuffer.h"
#include "MediaOutputBuffer.h"

extern "C" {
#include "user_interface.h"
}

// WiFi Settings
//#define WIFI_SSID "Henry's Netgear"
#define WIFI_SSID "Henry's iPhone 6"
#define WIFI_PWD "13913954971"

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
void setupWiFi();
bool playLocalFile(String path);
bool playRemoteUrl(String url);


void setup()
{
    Serial.begin(115200);

    // Set to 160 MHz in order to get better I/O performance
    // With ESP8266 running at 80 MHz, it is capable of handling up to 256 kb bitrate.
    // With ESP8266 running at 160 MHz, it is capable of handling up to 320 kb bitrate.
    log("Setting CPU frequency to 160Mhz...");
    system_update_cpu_freq(160);

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
    vs1053.setVolume(100);

    // playLocalFile("/test.mp3");
    playRemoteUrl("http://lhttp.qingting.fm/live/387/64k.mp3");
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


void setupWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PWD);
    // log("Connecting to %s", WIFI_STA);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    // log(WiFi.localIP().toString());
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

bool playRemoteUrl(String url)
{
    log("Loading %s...");
    const String host = "lhttp.qingting.fm";
    const int port = 80;
    const String path = "/live/387/64k.mp3";
    if (httpClient.connect(host.c_str(), port))
    {
        httpClient.print(String("GET ") + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +
                         "Icy-MetaData:1\r\n" + "Connection: close\r\n\r\n");
        mediaInputStream = &httpClient;
        log("%s has been loaded.", url.c_str());
        return true;
    }
    return false;
}
