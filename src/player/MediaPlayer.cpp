#include "MediaPlayer.h"

#include <Console.h>
#include <URLParser.h>

#include "../stream/LocalMediaStream.h"
#include "../stream/HTTPMediaStream.h"

MediaPlayer::MediaPlayer(VS1053 *vs1053)
{
    this->vs1053 = vs1053;
}

bool MediaPlayer::open(String location)
{
    if (location.startsWith("/"))
    {
        mediaStream = new LocalMediaStream(location);
    }
    else
    {
        URL url = parseURL(location);
        if (url.isValid)
        {
            if (url.protocol.equalsIgnoreCase("http"))
            {
                mediaStream = new HTTPMediaStream(location);
            }
            else
            {
                Console::error("Invalid protocol. Currently SmartRadio only support HTTP protocol.");
                return false;
            }
        }
        else
        {
            Console::error("Invalid URL.");
            return false;
        }
    }

    if (mediaStream && mediaStream->isValid())
    {
        Console::debug("File size: %d", mediaStream->totalSize());
        return true;
    }
    else
    {
        return false;
    }
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
