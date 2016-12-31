#include <Console.h>

#include "LocalMediaStream.h"

LocalMediaStream::LocalMediaStream(String path)
{
    Console::info("Loading %s...", path.c_str());
    fileStream = SPIFFS.open(path, "r"); // Open the file
    if (!fileStream)
    {
        Console::info("Error opening file %s", path.c_str()); // No luck
        return;
    }
    Console::info("%s has been loaded.", path.c_str());
    Console::debug("File size: %d", fileStream.size());
}

int LocalMediaStream::available()
{
    return fileStream.available();
}

int LocalMediaStream::read()
{
    return fileStream.read();
}

int LocalMediaStream::peek()
{
    return fileStream.peek();
}

void LocalMediaStream::flush()
{
    fileStream.flush();
}
