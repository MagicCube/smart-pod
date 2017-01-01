#ifndef RADIO_POD_H
#define RADIO_POD_H

#include <Arduino.h>

#include "BasePod.h"

class RadioPod : public BasePod
{
public:
    RadioPod(MediaPlayer *mediaPlayer);

    void activate() override;
    void play() override;
    void pause() override;
    void stop() override;
    void next() override;
    void prev() override;
};

#endif
