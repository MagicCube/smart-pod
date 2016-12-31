#include <Console.h>

#include "LocalMediaStream.h"

LocalMediaStream::LocalMediaStream()
{

}

bool LocalMediaStream::open(String path)
{
    if (!_closed)
    {
        close();
    }

    Console::info("Loading %s...", path.c_str());
    _fileStream = SPIFFS.open(path, "r"); // Open the file
    _totalSize = _fileStream.size();
    if (!_fileStream)
    {
        Console::info("Error opening file %s", path.c_str());
        _closed = true;
        _valid = false;
        return false;
    }
    else
    {
        Console::info("%s has been loaded.", path.c_str());
        Console::debug("File size: %d", _fileStream.size());
        _closed = false;
        _valid = true;
        return true;
    }
}

void LocalMediaStream::close()
{
    if (!_closed)
    {
        _fileStream.close();
        Console::info("LocalMediaStream closed.");
    }
    _closed = true;
    _valid = false;
    _totalSize = 0;
}

int LocalMediaStream::available()
{
    return !_closed ? _fileStream.available() : 0;
}

int LocalMediaStream::read()
{
    if (!_closed)
    {
        return _fileStream.read();
    }
    else
    {
        return 0;
    }
}

int LocalMediaStream::peek()
{
    if (!_closed)
    {
        return _fileStream.peek();
    }
    else
    {
        return 0;
    }
}

void LocalMediaStream::flush()
{
    if (!_closed)
    {
        _fileStream.flush();
    }
}




int LocalMediaStream::totalSize()
{
    return _totalSize;
}
