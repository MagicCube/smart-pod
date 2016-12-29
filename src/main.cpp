// Wiring:
// ESP   Wired to VS1053
// --- -------------------
// D0  DCS
// D1  CS
// D2  DREQ
// D5  SCK
// D6  MISO
// D7  MOSI
// --- -------------------
// RST RESET


// Flash Layout of ESP8266
// |--------------|-------|---------------|--|--|--|--|--|
// ^              ^       ^               ^     ^
// Sketch    OTA update   File system   EEPROM  WiFi config (SDK)

#include <Arduino.h>
#include <FS.h>
#include <SPI.h>

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include <log.h>
#include <VS1053.h>

#include "MediaInputBuffer.h"
#include "MediaOutputBuffer.h"

extern "C"
{
    #include "user_interface.h"
}

// WiFi Settings
#define WIFI_SSID "Henry's Netgear"
#define WIFI_PWD "13913954971"

// VS1053 Settings
#define VS1053_CS_PIN D1
#define VS1053_DCS_PIN D0
#define VS1053_DREQ_PIN D2

// Global Variables

// Global Objects
VS1053 vs1053player(VS1053_CS_PIN, VS1053_DCS_PIN, VS1053_DREQ_PIN);

MediaInputBuffer mediaInputBuffer;                   // Buffer for input stream from file or network
MediaOutputBuffer mediaOutputBuffer(&vs1053player);  // Buffer for output stream to VS1053

Stream *mediaInputStream = NULL;                     // mediaInputStream could be pointed to a FileInputStream or NetworkInputStream
File fileInputStream;                                // Input stream for local files.


// Function Declarations
void setupWiFi();
bool playLocalFile(String path);



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

    //log("Setting up WiFi...");
    //setupWiFi();
    //log("Setting up OTA...");
    //ArduinoOTA.begin();

    // Setup VS1053
    SPI.begin();
    vs1053player.begin();
    // Set the initial volume
    vs1053player.setVolume(100);

    log("Playing local media file...");
    playLocalFile("/test.mp3");
}

void loop()
{
    // Handling OTA request
    //ArduinoOTA.handle();

    // ****** READING FROM INPUT STREAM *******
    // When mediaInputStream has been assigned
    if (mediaInputStream)
    {
        mediaInputBuffer.readFromStream(mediaInputStream);
    }

    // ********* KEEP VS1053 FILLED **********
    while (vs1053player.data_request() && mediaInputBuffer.available())
    {
        mediaOutputBuffer.write(mediaInputBuffer.read());
    }
}








void setupWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PWD);
    log("Connecting to %s", WIFI_STA);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    log(WiFi.localIP().toString());
}

bool playLocalFile(String path)
{
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
