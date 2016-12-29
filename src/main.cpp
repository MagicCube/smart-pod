//******************************************************************************************
//*  Esp_radio -- Webradio receiver for ESP8266, 1.8 color display and VS1053 MP3 module.  *
//*  With ESP8266 running at 80 MHz, it is capable of handling up to 256 kb bitrate.       *
//*  With ESP8266 running at 160 MHz, it is capable of handling up to 320 kb bitrate.      *
//******************************************************************************************
//
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


#include <Arduino.h>
#include <FS.h>
#include <SPI.h>

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include <VS1053.h>
#include <dbgprint.h>

#include "InputBuffer.h"

extern "C"
{
    #include "user_interface.h"
}

enum DataMode
{
    INIT = 1,
    HEADER = 2,
    DATA = 4,
    METADATA = 8,
    PLAYLISTINIT = 16,
    PLAYLISTHEADER = 32,
    PLAYLISTDATA = 64,
    STOPREQD = 128,
    STOPPED = 256
}; // State for datastream


// WiFi Settings
#define WIFI_SSID "Henry's Netgear"
#define WIFI_PWD "13913954971"

// VS1053 Settings
#define VS1053_CS_PIN D1
#define VS1053_DCS_PIN D0
#define VS1053_DREQ_PIN D2

// Global Variables
DataMode dataMode;       // State of datastream
uint32_t totalcount = 0; // Counter mp3 dat\\pl.0
int metacount;           // Number of bytes in metadata
int metaint = 0;         // Number of databytes between metadata
char metaline[200];      // Readable line in metadata
char streamtitle[150];   // Streamtitle from metadata
char icyname[50];        // Station name
int datacount;           // Counter databytes before metadata
int bitrate;             // Bitrate in kb/sec

// Global Objects
File mp3file;
VS1053 vs1053player(VS1053_CS_PIN, VS1053_DCS_PIN, VS1053_DREQ_PIN);
InputBuffer inputBuffer;


// Function Declarations
void setupWiFi();
bool playLocalFile(String path);
bool chkhdrline(const char *str);
void handleByte(uint8_t b, bool force = false);



void setup()
{
    Serial.begin(115200);

    system_update_cpu_freq(160);    // Set to 160 MHz in order to get better I/O performance

    SPIFFS.begin();
    //setupWiFi();
    //ArduinoOTA.begin();

    SPI.begin();
    vs1053player.begin();
    vs1053player.setVolume(100);

    playLocalFile("/test.mp3");
}

void loop()
{
    //ArduinoOTA.handle();

    int maxChunkSize;
    maxChunkSize = mp3file.available();
    if (maxChunkSize > 1024) // Reduce byte count for this loop()
    {
        maxChunkSize = 1024;
    }

    while (inputBuffer.hasSpace() && maxChunkSize--)
    {
        inputBuffer.write(mp3file.read()); // Yes, store one byte in inputBuffer
    }

    //yield();

    while (vs1053player.data_request() && inputBuffer.available()) // Try to keep VS1053 filled
    {
         handleByte(inputBuffer.read()); // Yes, handle it
    }
}








void setupWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PWD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println(WiFi.localIP().toString());
}

bool playLocalFile(String path)
{
    mp3file = SPIFFS.open(path, "r"); // Open the file
    if (!mp3file)
    {
        dbgprint("Error opening file %s", path.c_str()); // No luck
        return false;
    }
    dbgprint("%s has been loaded.", path.c_str());
    dataMode = INIT;
    return true;
}

bool chkhdrline(const char *str)
{
    char b;      // Byte examined
    int len = 0; // Lengte van de string

    while ((b = *str++)) // Search to end of string
    {
        len++;           // Update string length
        if (!isalpha(b)) // Alpha (a-z, A-Z)
        {
            if (b != '-') // Minus sign is allowed
            {
                if (b == ':') // Found a colon?
                {
                    return ((len > 5) && (len < 50)); // Yes, okay if length is okay
                }
                else
                {
                    return false; // Not a legal character
                }
            }
        }
    }
    return false; // End of string without colon
}

void handleByte(uint8_t b, bool force)
{
    static uint16_t metaindex;                          // Index in metaline
    static uint16_t playlistcnt;                        // Counter to find right entry in playlist
    static bool firstmetabyte;                          // True if first metabyte (counter)
    static int LFcount;                                 // Detection of end of header
    static __attribute__((aligned(4))) uint8_t buf[32]; // Buffer for chunk
    static int chunkcount = 0;                          // Data in chunk
    static bool firstchunk = true;                      // First chunk as input
    char *p;                                            // Pointer in metaline

    if (dataMode == INIT) // Initialize for header receive
    {
        metaint = 0;       // No metaint found
        LFcount = 0;       // For detection end of header (with 2 linebreaks)
        bitrate = 0;       // Bitrate still unknown
        metaindex = 0;     // Prepare for new line
        dataMode = DATA; // Handle header
        totalcount = 0;    // Reset totalcount
    }
    if (dataMode == DATA) // Handle next byte of MP3/Ogg data
    {
        buf[chunkcount++] = b;                  // Save byte in chunkbuffer
        if (chunkcount == sizeof(buf) || force) // Buffer full?
        {
            if (firstchunk)
            {
                firstchunk = false;
                dbgprint("First chunk:");   // Header for printout of first chunk
                // for (i = 0; i < 32; i += 8) // Print 4 lines
                // {
                //     dbgprint("%02X %02X %02X %02X %02X %02X %02X %02X", buf[i], buf[i + 1],
                //              buf[i + 2], buf[i + 3], buf[i + 4], buf[i + 5], buf[i + 6],
                //              buf[i + 7]);
                // }
            }
            vs1053player.playChunk(buf, chunkcount); // Yes, send to player
            chunkcount = 0;                          // Reset count
        }
        totalcount++;     // Count number of bytes, ignore overflow
        if (metaint != 0) // No METADATA on Ogg streams
        {
            if (--datacount == 0) // End of datablock?
            {
                if (chunkcount) // Yes, still data in buffer?
                {
                    vs1053player.playChunk(buf, chunkcount); // Yes, send to player
                    chunkcount = 0;                          // Reset count
                }
                dataMode = METADATA;
                firstmetabyte = true; // Expecting first metabyte (counter)
            }
        }
    }
}
