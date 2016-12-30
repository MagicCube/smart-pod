#include "MediaPlayer.h"

#include <VS1053.h>

#include <Console.h>
#include <URLParser.h>

MediaPlayer::MediaPlayer(VS1053 *vs1053) : mediaOutputBuffer(vs1053)
{
    this->vs1053 = vs1053;
}

bool MediaPlayer::begin()
{
    bool vs1053Enabled = vs1053->begin();
    if (!vs1053Enabled)
    {
        return false;
    }

    // Set the initial volume
    setVolume(70);

    return true;
}


bool MediaPlayer::handle()
{
    // ****** READING FROM INPUT STREAM *******
    // When currentInputStream has been assigned
    if (currentInputStream)
    {
        generalInputBuffer.readFromStream(currentInputStream);
    }

    // ********* KEEP VS1053 FILLED **********
    while (vs1053->data_request() && generalInputBuffer.available())
    {
        mediaOutputBuffer.write(generalInputBuffer.read());
    }

    return true;
}

void MediaPlayer::setVolume(int volume)
{
    vs1053->setVolume(volume);
}


bool MediaPlayer::loadLocalFile(String path)
{
    Console::info("Loading %s...", path.c_str());
    fileInputStream = SPIFFS.open(path, "r"); // Open the file
    if (!fileInputStream)
    {
        Console::info("Error opening file %s", path.c_str()); // No luck
        return false;
    }
    currentInputStream = &fileInputStream;
    Console::info("%s has been loaded.", path.c_str());
    return true;
}

bool MediaPlayer::loadRemoteURL(String urlString)
{
    Console::info("Loading %s...", urlString.c_str());
    URL url = parseURL(urlString);
    if (url.isValid)
    {
        if (url.protocol.equals("http"))
        {
            if (httpClient.connect(url.host.c_str(), url.port))
            {
                httpClient.print(String("GET ") + url.path + " HTTP/1.1\r\n" + "Host: " + url.host +
                                 "\r\n" + "Connection: close\r\n\r\n");
                currentInputStream = &httpClient;
                Console::info("%s has been loaded.", urlString.c_str());
                return true;
            }
        }
        else
        {
            Console::error("Invalid protocol. Currently we only support HTTP protocol.");
        }
    }
    else
    {
        Console::error("Invalid URL.");
    }
    return false;
}
