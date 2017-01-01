#ifndef FAV_MUSIC_POD_H
#define FAV_MUSIC_POD_H

#include <Arduino.h>

#include "BasePod.h"

class FavMusicPod : public BasePod
{
public:
    FavMusicPod(MediaPlayer *mediaPlayer);

    void activate() override;
    void play() override;
    void pause() override;
    void stop() override;
    void next() override;
    void prev() override;
};

#endif
