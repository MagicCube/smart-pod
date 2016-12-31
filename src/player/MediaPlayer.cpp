#include "MediaPlayer.h"

#include "../stream/LocalMediaStream.h"

MediaPlayer::MediaPlayer(VS1053 *vs1053)
{
    this->vs1053 = vs1053;
}

bool MediaPlayer::open(String path)
{
    mediaStream = new LocalMediaStream(path);
    return true;
}

void MediaPlayer::handle()
{
    static __attribute__((aligned(4))) char buffer[MEDIA_PLAYER_BUFFER_SIZE];

    // First we need to confirm whether VS1053 is enable to receive data.
    if (vs1053->data_request())
    {
        size_t len = mediaStream->available();
        if (len <= 0)
        {
            return;
        }

        // Check if the buffer is too small for inputStream
        // In most case, it is.
        if (len > MEDIA_PLAYER_BUFFER_SIZE)
        {
            len = MEDIA_PLAYER_BUFFER_SIZE;
        }

        len = mediaStream->readBytes(buffer, len);
        vs1053->playChunk((uint8 *)buffer, len);
    }
}
