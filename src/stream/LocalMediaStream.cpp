#include <Console.h>

#include "LocalMediaStream.h"

LocalMediaStream::LocalMediaStream(String path)
{
    Console::info("Loading %s...", path.c_str());
    _fileStream = SPIFFS.open(path, "r"); // Open the file
    if (!_fileStream)
    {
        Console::info("Error opening file %s", path.c_str());
        this->setValid(false);
    }
    else
    {
        Console::info("%s has been loaded.", path.c_str());
        Console::debug("File size: %d", _fileStream.size());
        this->setValid(true);
    }
}

int LocalMediaStream::available()
{
    return _fileStream.available();
}

int LocalMediaStream::read()
{
    return _fileStream.read();
}

int LocalMediaStream::peek()
{
    return _fileStream.peek();
}

void LocalMediaStream::flush()
{
    _fileStream.flush();
}




size_t LocalMediaStream::totalSize()
{
    return _fileStream.size();
}
