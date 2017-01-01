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


    virtual void activate() = 0;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void next() = 0;
    virtual void prev() = 0;

    virtual void playPause()
    {
        if (isPlaying())
        {
            pause();
        }
        else
        {
            play();
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

    void setVolumeUp()
    {
        setVolume(_volume + 5);
    }
    void setVolumeDown()
    {
        setVolume(_volume - 5);
    }

protected:
    bool _playing = false;
    uint8_t _volume = 80;

    MediaPlayer* _mediaPlayer;
};

#endif
