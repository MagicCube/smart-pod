#include "MediaPlayer.h"

#include <Console.h>
#include <URLParser.h>

MediaPlayer::MediaPlayer(VS1053 *vs1053)
{
    _vs1053 = vs1053;
}

VS1053* MediaPlayer::getVS1053()
{
    return _vs1053;
}

bool MediaPlayer::open(String location)
{
    close();

    if (location.startsWith("/"))
    {
        if (!_localMediaStream)
        {
            _localMediaStream = new LocalMediaStream();
        }
        _mediaStream = _localMediaStream;
        _mediaStream->open(location);
    }
    else
    {
        URL url = parseURL(location);
        if (url.isValid)
        {
            if (url.protocol.equalsIgnoreCase("http"))
            {
                if (!_httpMediaStream)
                {
                    _httpMediaStream = new HTTPMediaStream();
                }
                _mediaStream = _httpMediaStream;
                _mediaStream->open(location);
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

void MediaPlayer::close()
{
    if (_mediaStream)
    {
        _mediaStream->close();
    }
}

bool MediaPlayer::isClosed()
{
    if (_mediaStream)
    {
        if (_mediaStream->isValid())
        {
            return _mediaStream->isClosed();
        }
    }
    return true;
}

void MediaPlayer::handle()
{
    _handleMediaStream();
}

void MediaPlayer::_handleMediaStream()
{
    if (!_mediaStream || !_mediaStream->isValid() || _mediaStream->isClosed()) return;

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
