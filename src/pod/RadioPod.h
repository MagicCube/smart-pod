#ifndef RADIO_POD_H
#define RADIO_POD_H

#include <Arduino.h>

#include "BaseListPod.h"

class RadioPod : public BaseListPod
{
public:
    RadioPod(MediaPlayer *mediaPlayer);

    void loadPlaylist();
    void pause() override;
    void stop() override;
};

#endif
