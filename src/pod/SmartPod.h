#ifndef SMART_POD_H
#define SMART_POD_H

#include <Arduino.h>

#include <VS1053.h>

#include "../wiring.h"
#include "./player/MediaPlayer.h"

#include "Pod.h"
#include "FavMusicPod.h"
#include "RadioPod.h"

typedef enum {
    RADIO_POD,
    FAV_MUSIC_POD
} SmartPodMode;

class SmartPod : public Pod
{
public:
    SmartPod();

    bool begin();
    void handle() override;

    void restart();
    void sleep();

    void switchMode(SmartPodMode mode);
    void switchMode();

    void activate() override;
    void deactivate() override;
    bool isPlaying() override;
    void play() override;
    void pause() override;
    void stop() override;
    void next() override;
    void prev() override;
    uint8_t getVolume() override;
    void setVolume(uint8_t volume) override;

    void playPause();
    void setVolumeUp();
    void setVolumeDown();

private:
    SmartPodMode _mode = RADIO_POD;
    VS1053 _vs1053;

    Pod* _activePod;
    RadioPod* _radioPod;
    FavMusicPod* _favMusicPod;
};

#endif
