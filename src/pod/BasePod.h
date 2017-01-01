#ifndef BASE_POD_H
#define BASE_POD_H

#include <Arduino.h>

#include "Pod.h"

class BasePod : public Pod
{
public:
    BasePod(MediaPlayer* mediaPlayer) : Pod(mediaPlayer)
    {

    }

    virtual bool isPlaying()
    {
        return _playing;
    }

    virtual void deactivate()
    {
        if (isPlaying())
        {
            stop();
        }
    }

    virtual void handle()
    {
        if (isPlaying())
        {
            _mediaPlayer->handle();
        }
    }


    virtual uint8_t getVolume()
    {
        return _volume;
    }

    virtual void setVolume(uint8_t volume)
    {
        if (volume > 100)
        {
            _volume = 100;
        }
        else if (volume < 0)
        {
            _volume = 0;
        }
        else
        {
            _volume = volume;
        }
        _mediaPlayer->getVS1053()->setVolume(_volume);
    }
};

#endif
