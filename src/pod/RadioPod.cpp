#include "RadioPod.h"

RadioPod::RadioPod(MediaPlayer *mediaPlayer) : BasePod(mediaPlayer)
{

}

void RadioPod::activate()
{
    setVolume(getVolume());
    play();
}

void RadioPod::play()
{
    if (!isPlaying())
    {
        _mediaPlayer->open("http://http.qingting.fm/387.mp3"); // NCR Finance
        _playing = true;
    }
}

void RadioPod::pause()
{
    stop();
}

void RadioPod::stop()
{
    if (isPlaying())
    {
        _playing = false;
    }
    _mediaPlayer->close();
}

void RadioPod::next()
{

}

void RadioPod::prev()
{

}
