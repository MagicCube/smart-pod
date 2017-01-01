#ifndef SMART_POD_H
#define SMART_POD_H

#include <Arduino.h>

#include <VS1053.h>

#include "wiring.h"
#include "./player/MediaPlayer.h"

typedef enum {
    RADIO,
    FAV_MUSIC_LIST
} SmartPodMode;

class SmartPod
{
public:
    SmartPod();

    bool begin();
    void handle();

    void switchMode(SmartPodMode mode);
    void switchMode();

    uint8_t getVolume();
    void setVolume(int volume);
    void setVolumeUp();
    void setVolumeDown();

private:
    SmartPodMode _mode = RADIO;
    VS1053 _vs1053;
    MediaPlayer _mediaPlayer;
};

#endif
