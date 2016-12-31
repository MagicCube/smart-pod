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
#include <ESP8266mDNS.h>
#include <FS.h>

// Libraries for Hardwares
#include <VS1053.h>

// Libraries by me
#include <Console.h>
#include <URLParser.h>

#include "./player/MediaPlayer.h"
#include "./wifi/WiFiConnector.h"
#include "./controller/ButtonController.h"

// You can find PIN wiring defininitions in "wiring.h"
#include "./wiring.h"

// In order to invoke system_update_cpu_freq(),
// we have to include "user_interface.h".
extern "C" {
#include "user_interface.h"
}






// Global Variables

// Global Objects
VS1053 vs1053(VS1053_XCS_PIN, VS1053_XDCS_PIN, VS1053_DREQ_PIN);
MediaPlayer mediaPlayer(&vs1053);
ButtonController buttonController;




void setup()
{
    Console::begin();
    Console::line();
    Console::info("SmartPod is now starting...");


    // ** Setup CPU **
    // Set to 160 MHz in order to get better I/O performance
    // Set to 80 MHz in order to save power
    // With ESP8266 running at 80 MHz, it is capable of handling up to 256 kb bitrate.
    // With ESP8266 running at 160 MHz, it is capable of handling up to 320 kb bitrate.
    const int CPU_SPEED = 80;
    Console::info("Setting CPU frequency to %d Mhz...", CPU_SPEED);
    system_update_cpu_freq(CPU_SPEED);



    /*
    // ** Setup File System **
    // Here we use SPIFFS(ESP8266 built-in File System) to store stations and other settings,
    // as well as short sound effects.
    Console::info("Setting up file system....");
    SPIFFS.begin();



    // ** Setup WiFi **
    // According to my personal experiments, WiFi must be started before VS1053,
    // otherwise WiFi could not be well connected.
    // I don't know why, maybe can be solved in the future.
    Console::info("Setting up WiFi...");
    bool wifiConnected = WiFiConnector::begin();
    if (!wifiConnected)
    {
        Console::fatal("SmartPod failed to start up.\nReason: No WiFi Connection.");
        return;
    }
    // Setup OTA firmware update
    Console::info("Setting up OTA...");
    ArduinoOTA.begin();





    // ** Setup VS1053 **
    SPI.begin();
    bool vs1053Enabled = vs1053.begin();
    if (!vs1053Enabled)
    {
        Console::fatal("SmartPod failed to start up.\nReason: VS1053 can not be started.");
        return;
    }
    // Set the initial volume
    vs1053.setVolume(90);
    */


    // Setup Buttons
    buttonController.begin();


    /**
     * YES, WE'RE READY TO PLAY!
     */
    Console::info("SmartPod is now running...");



    //mediaPlayer.open("/test.mp3");
    //mediaPlayer.open("http://http.qingting.fm/387.mp3"); // NCR Finance
    //mediaPlayer.open("http://http.qingting.fm/4963.mp3"); // Nanjing Music Radio
    //mediaPlayer.open("http://m2.music.126.net/NG4I9FVAm9jCQCvszfLB8Q==/1377688074172063.mp3");
}

void loop()
{
    buttonController.handle();

    //ArduinoOTA.handle();

    //buttonController.handle();

    //mediaPlayer.handle();
}
