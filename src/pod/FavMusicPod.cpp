#include <Console.h>

#include "FavMusicPod.h"

FavMusicPod::FavMusicPod(MediaPlayer *mediaPlayer) : BaseListPod(mediaPlayer)
{

}

void FavMusicPod::loadPlaylist()
{
    loadPlayListFromLocal("/fav-music-pod.m3u");
}
