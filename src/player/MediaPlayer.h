#ifndef MEDIA_PLAYER_H
#define MEDIA_PLAYER_H

#include <Arduino.h>

#include <VS1053.h>

#include "../stream/MediaStream.h"

#define MEDIA_PLAYER_BUFFER_SIZE 32

class MediaPlayer
{
public:
    MediaPlayer(VS1053* vs1053);

    bool open(String location);
    void handle();

protected:
    VS1053* _vs1053;
    MediaStream* _mediaStream;

    void _handleMediaStream();
};

#endif
