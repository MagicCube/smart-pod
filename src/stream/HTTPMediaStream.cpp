#include <Console.h>

#include "HTTPMediaStream.h"

HTTPMediaStream::HTTPMediaStream(String url)
{
    Console::info("Loading %s...", url.c_str());
    bool result = _httpClient.begin(url);
    if (!result)
    {
        Console::error("Can not open %s.", url.c_str());
        setValid(false);
    }
    else
    {
        _httpStream = _httpClient.getStreamPtr();
        int httpCode = _httpClient.GET();
        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK)
            {
                _httpStream = _httpClient.getStreamPtr();
                Console::info("%s has been loaded.", url.c_str());

                if (_httpClient.getTransferEncoding() == HTTPC_TE_IDENTITY)
                {
                    _totalSize = _httpClient.getSize();
                }
                else if (_httpClient.getTransferEncoding() == HTTPC_TE_CHUNKED)
                {
                    Console::debug("Transfer-encoding: chunked");
                    _totalSize = -1;

                    _readChunkSize();
                }
                setValid(true);
            }
            else
            {
                Console::error("%s can not be loaded.", url.c_str());
                Console::error("The server response with HTTP code %d", httpCode);
                setValid(false);
            }
        }
        else
        {
            Console::error("Can not open %s.", url.c_str());
            setValid(false);
        }
    }
}

int HTTPMediaStream::available()
{
    return _httpStream->available();
}

int HTTPMediaStream::read()
{
    _chunkIndex++;
    if (_chunkIndex >= _chunkSize - 1 && _httpStream->peek() == '\r')
    {
        _httpStream->read();
        _httpStream->read();
        _readChunkSize();
        _chunkIndex++;
    }
    return _httpStream->read();
}

int HTTPMediaStream::peek()
{
    return _httpStream->peek();
}

void HTTPMediaStream::flush()
{
    _httpStream->flush();
}

int HTTPMediaStream::totalSize()
{
    return _totalSize;
}


int parseHex(const char *str)
{
  return (int) strtol(str, 0, 16);
}

void HTTPMediaStream::_readChunkSize()
{
    String chunkSizeStr = _httpStream->readStringUntil('\n');
    _chunkSize = parseHex(chunkSizeStr.c_str());
    _chunkIndex = -1;
    //Console::debug("Chunk size in hex: %s", chunkSizeStr.c_str());
    //Console::debug("Chunk size: %d", _chunkSize);
}
