#ifndef FAV_MUSIC_POD_H
#define FAV_MUSIC_POD_H

#include <Arduino.h>

#include "BaseListPod.h"

class FavMusicPod : public BaseListPod
{
public:
    FavMusicPod(MediaPlayer *mediaPlayer);

    void loadPlaylist() override;
};

#endif
