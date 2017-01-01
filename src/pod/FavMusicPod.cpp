#include "FavMusicPod.h"

FavMusicPod::FavMusicPod(MediaPlayer *mediaPlayer) : Pod(mediaPlayer)
{

}

void FavMusicPod::activate()
{
    setVolume(getVolume());
    _mediaPlayer->open("http://m2.music.126.net/NG4I9FVAm9jCQCvszfLB8Q==/1377688074172063.mp3");
    play();
}

void FavMusicPod::play()
{
    if (!isPlaying())
    {
        _playing = true;
    }
}

void FavMusicPod::pause()
{
    if (isPlaying())
    {
        _playing = false;
    }
}

void FavMusicPod::stop()
{
    pause();
    _mediaPlayer->close();
}

void FavMusicPod::next()
{
    _mediaPlayer->open("http://m2.music.126.net/AYnQ7kVZXTFei0RtQ_PGTA==/3244658815248018.mp3");
    play();
}

void FavMusicPod::prev()
{
    _mediaPlayer->open("http://m2.music.126.net/NG4I9FVAm9jCQCvszfLB8Q==/1377688074172063.mp3");
    play();
}
