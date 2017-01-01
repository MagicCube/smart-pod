#include <Console.h>

#include "FavMusicPod.h"

FavMusicPod::FavMusicPod(MediaPlayer *mediaPlayer) : BaseListPod(mediaPlayer)
{

}

void FavMusicPod::loadPlaylist()
{
    Console::info("Loading playlist of FavMusicPod...");
    clearPlaylist();
    addToPlaylist("http://m2.music.126.net/AYnQ7kVZXTFei0RtQ_PGTA==/3244658815248018.mp3");
    addToPlaylist("http://m2.music.126.net/NG4I9FVAm9jCQCvszfLB8Q==/1377688074172063.mp3");
    Console::info("Playlist of FavMusicPod has been loaded.");
}
