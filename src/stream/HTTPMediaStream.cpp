#include <Console.h>

#include "HTTPMediaStream.h"

HTTPMediaStream::HTTPMediaStream(String url)
{
    Console::info("Loading %s...", url.c_str());
    bool result = httpClient.begin(url);
    if (!result)
    {
        Console::error("Can not open %s.", url.c_str());
        setValid(false);
    }
    else
    {
        httpStream = httpClient.getStreamPtr();
        int httpCode = httpClient.GET();
        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK)
            {
                Console::info("%s has been loaded.", url.c_str());

                if (httpClient.getTransferEncoding() == HTTPC_TE_IDENTITY)
                {
                    _totalSize = httpClient.getSize();
                }
                else if (httpClient.getTransferEncoding() == HTTPC_TE_CHUNKED)
                {
                    Console::debug("Transfer-encoding: chunked");
                    _totalSize = -1;
                }
                httpStream = httpClient.getStreamPtr();
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
    return httpStream->available();
}

int HTTPMediaStream::read()
{
    return httpStream->read();
}

int HTTPMediaStream::peek()
{
    return httpStream->peek();
}

void HTTPMediaStream::flush()
{
    httpStream->flush();
}

size_t HTTPMediaStream::totalSize()
{
    return _totalSize;
}
