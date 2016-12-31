#include "MediaStream.h"

size_t MediaStream::write(uint8_t byte)
{
    // MediaInputStream is read-only.
    return 0;
}

bool MediaStream::isValid()
{
    return valid;
}

void MediaStream::setValid(bool valid)
{
    this->valid = valid;
}
