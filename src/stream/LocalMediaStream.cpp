#include <Console.h>

#include "LocalMediaStream.h"

LocalMediaStream::LocalMediaStream(String path)
{
    Console::info("Loading %s...", path.c_str());
    inputStream = SPIFFS.open(path, "r"); // Open the file
    if (!inputStream)
    {
        Console::info("Error opening file %s", path.c_str()); // No luck
        return;
    }
    Console::info("%s has been loaded.", path.c_str());
    Console::debug("File size: %d", inputStream.size());
}

int LocalMediaStream::available()
{
    return inputStream.available();
}

int LocalMediaStream::read()
{
    return inputStream.read();
}

int LocalMediaStream::peek()
{
    return inputStream.peek();
}

void LocalMediaStream::flush()
{
    inputStream.flush();
}
