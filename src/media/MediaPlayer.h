#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <FS.h>

#include "../stream/GeneralInputBuffer.h"
#include "MediaOutputBuffer.h"

class VS1053;

class MediaPlayer
{
  public:
    MediaPlayer(VS1053 *vs1053);

    bool begin();
    bool handle();

    void setVolume(int volume);

    bool loadRemoteURL(String url);
    bool loadLocalFile(String path);

  private:
    VS1053 *vs1053;
    // currentInputStream could be pointed to a FileInputStream or NetworkInputStream
    Stream *currentInputStream = NULL;
    // Input stream for HTTP.
    WiFiClient httpClient;
    // Input stream for local files.
    File fileInputStream;
    // Buffer for input stream from file or network
    GeneralInputBuffer generalInputBuffer;
    // Buffer for output stream to VS1053
    MediaOutputBuffer mediaOutputBuffer;
};

#endif
