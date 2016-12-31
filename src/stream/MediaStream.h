#ifndef MEDIA_STREAM_H
#define MEDIA_STREAM_H

#include "Arduino.h"

// Abstract
class MediaStream : public Stream
{
public:
    bool isValid();
    bool isClosed();
    size_t write(uint8_t byte) override;

    virtual bool open(String location) = 0;
    virtual void close() = 0;

    virtual int totalSize() = 0;

protected:
    bool _valid = false;
    bool _closed = true;
};


#endif
