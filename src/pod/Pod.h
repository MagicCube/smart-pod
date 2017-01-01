#ifndef POD_H
#define POD_H

#include <Arduino.h>

#include <VS1053.h>
#include "../player/MediaPlayer.h"

class Pod
{
public:
    Pod(MediaPlayer* mediaPlayer)
    {
        _mediaPlayer = mediaPlayer;
    }

    virtual void handle() = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual bool isPlaying() = 0;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void next() = 0;
    virtual void prev() = 0;
    virtual uint8_t getVolume() = 0;
    virtual void setVolume(uint8_t volume) = 0;

protected:
    bool _playing = false;
    uint8_t _volume = 80;

    MediaPlayer* _mediaPlayer;
};

#endif
