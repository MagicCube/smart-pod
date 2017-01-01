#include "RadioPod.h"

RadioPod::RadioPod(MediaPlayer *mediaPlayer) : BaseListPod(mediaPlayer)
{

}

void RadioPod::loadPlaylist()
{
    loadPlayListFromLocal("/radio-pod.m3u");
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
