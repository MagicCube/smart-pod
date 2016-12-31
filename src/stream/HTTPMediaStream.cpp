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
        Console::info("%s has been loaded.", url.c_str());
        httpStream = httpClient.getStreamPtr();
        int httpCode = httpClient.GET();
        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK)
            {
                if (httpClient.getTransferEncoding() == HTTPC_TE_CHUNKED)
                {
                    Console::debug("[Chunked] transfer-encoding header was found.");
                }
                httpStream = httpClient.getStreamPtr();
                Console::info("%s has been loading.", url.c_str());
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
