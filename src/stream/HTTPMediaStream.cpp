#include <Console.h>

#include "HTTPMediaStream.h"

HTTPMediaStream::HTTPMediaStream()
{

}

bool HTTPMediaStream::open(String url)
{
    Console::info("Loading %s...", url.c_str());
    bool result = _httpClient.begin(url);
    if (!result)
    {
        Console::error("Can not open %s.", url.c_str());
        _valid = false;
        _closed = true;
        return false;
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
                    _totalSize = 0;
                    _chunkSize = 0;

                    _readChunkSize();
                }
                _valid = true;
                _closed = false;
                return true;
            }
            else
            {
                Console::error("%s can not be loaded.", url.c_str());
                Console::error("The server response with HTTP code %d", httpCode);
                _valid = false;
                _closed = true;
                return false;
            }
        }
        else
        {
            Console::error("Can not open %s.", url.c_str());
            _valid = false;
            _closed = true;
            return false;
        }
    }
}

void HTTPMediaStream::close()
{
    if (!_closed)
    {
        Console::info("Closing HTTPMediaStream...");
        _httpClient.end();
        if (_httpStream)
        {
            _httpStream->flush();
            _httpStream->stop();
            while (_httpStream->connected())
            {
                _httpStream->flush();
                _httpStream->stop();
                delay(100);
            }
        }
        Console::info("HTTPMediaStream closed.");
    }
    _valid = false;
    _closed = true;
    _totalSize = 0;
    _chunkSize = 0;
    _chunkIndex = 0;
}

int HTTPMediaStream::available()
{
    if (!_closed)
    {
        return _httpStream->available();
    }
    else
    {
        return 0;
    }
}

int HTTPMediaStream::read()
{
    if (!_closed && _httpStream)
    {
        if (_httpClient.getTransferEncoding() == HTTPC_TE_CHUNKED)
        {
            _chunkIndex++;
            if (_chunkIndex >= _chunkSize - 1 && _httpStream->peek() == '\r')
            {
                _httpStream->read();
                _httpStream->read();
                _readChunkSize();
                _chunkIndex++;
            }
        }
        return _httpStream->read();
    }
    else
    {
        return 0;
    }
}

int HTTPMediaStream::peek()
{
    if (!_closed && _httpStream)
    {
        return _httpStream->peek();
    }
    else
    {
        return 0;
    }
}

void HTTPMediaStream::flush()
{
    if (!_closed && _httpStream)
    {
        _httpStream->flush();
    }
}

int HTTPMediaStream::totalSize()
{
    return _totalSize;
}


int __parseHex(const char *str)
{
  return (int) strtol(str, 0, 16);
}

void HTTPMediaStream::_readChunkSize()
{
    // plus the last chunkSize
    String chunkSizeStr = _httpStream->readStringUntil('\n');
    _chunkSize = __parseHex(chunkSizeStr.c_str());
    _chunkIndex = -1;
    _totalSize += _chunkSize;

    //Console::debug("Chunk size in hex: %s", chunkSizeStr.c_str());
    //Console::debug("Chunk size: %d", _chunkSize);
}
