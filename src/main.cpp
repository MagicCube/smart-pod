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

#include "./SmartPod.h"
#include "./controller/ButtonController.h"
#include "./wifi/WiFiConnector.h"

// In order to invoke system_update_cpu_freq(),
// we have to include "user_interface.h".
extern "C" {
#include "user_interface.h"
}






// Global Variables

// Global Objects
SmartPod smartPod;
ButtonController buttonController(&smartPod);



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
        Console::fatal("SmartPod failed to start up.");
        return;
    }
    // Setup OTA firmware update
    //Console::info("Setting up OTA...");
    //ArduinoOTA.begin();





    // ** Setup VS1053 **
    SPI.begin();
    bool smartPodEnabled = smartPod.begin();
    if (!smartPodEnabled)
    {
        Console::fatal("SmartPod failed to start up.");
        return;
    }


    // Setup Buttons
    buttonController.begin();


    /**
     * YES, WE'RE READY TO PLAY!
     */
    Console::info("SmartPod is now running...");

    smartPod.switchMode(RADIO);
}

void loop()
{
    //buttonController.handle();
    //ArduinoOTA.handle();
    buttonController.handle();
    smartPod.handle();
}
