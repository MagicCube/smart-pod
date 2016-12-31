#ifndef MEDIA_STREAM_H
#define MEDIA_STREAM_H

#include "Arduino.h"

// Abstract
class MediaStream : public Stream
{
    size_t write(uint8_t byte) override;
};


#endif
