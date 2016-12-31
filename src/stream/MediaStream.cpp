#include "MediaStream.h"

size_t MediaStream::write(uint8_t byte)
{
    // MediaInputStream is read-only.
    return 0;
}

bool MediaStream::isValid()
{
    return _valid;
}

void MediaStream::setValid(bool valid)
{
    _valid = valid;
}
