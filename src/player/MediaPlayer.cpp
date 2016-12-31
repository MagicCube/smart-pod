#include "MediaPlayer.h"

#include <Console.h>
#include <URLParser.h>

#include "../stream/LocalMediaStream.h"
#include "../stream/HTTPMediaStream.h"

MediaPlayer::MediaPlayer(VS1053 *vs1053)
{
    _vs1053 = vs1053;
}

bool MediaPlayer::open(String location)
{
    if (location.startsWith("/"))
    {
        _mediaStream = new LocalMediaStream(location);
    }
    else
    {
        URL url = parseURL(location);
        if (url.isValid)
        {
            if (url.protocol.equalsIgnoreCase("http"))
            {
                _mediaStream = new HTTPMediaStream(location);
            }
            else
            {
                Console::error("Invalid protocol. Currently SmartPod only support HTTP protocol.");
                return false;
            }
        }
        else
        {
            Console::error("Invalid URL.");
            return false;
        }
    }

    if (_mediaStream && _mediaStream->isValid())
    {
        Console::debug("File size / First chunk size: %d", _mediaStream->totalSize());
        return true;
    }
    else
    {
        return false;
    }
}

void MediaPlayer::handle()
{
    _handleMediaStream();
}

void MediaPlayer::_handleMediaStream()
{
    if (!_mediaStream) return;

    static __attribute__((aligned(4))) char buffer[MEDIA_PLAYER_BUFFER_SIZE];

    // First we need to confirm whether VS1053 is enable to receive data.
    if (_vs1053->data_request())
    {
        size_t len = _mediaStream->available();
        if (len == 0)
        {
            return;
        }

        // Check if the buffer is too small for inputStream
        // In most case, it is.
        if (len > MEDIA_PLAYER_BUFFER_SIZE)
        {
            len = MEDIA_PLAYER_BUFFER_SIZE;
        }

        len = _mediaStream->readBytes(buffer, len);
        if (len > 0)
        {
            _vs1053->playChunk((uint8 *)buffer, len);
        }
    }
}
