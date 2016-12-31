#include <VS1053.H>

#include "MediaOutputBuffer.h"

MediaOutputBuffer::MediaOutputBuffer(VS1053 *vs1053)
{
    this->vs1053 = vs1053;
}

void MediaOutputBuffer::write(uint8_t byte, bool force)
{
    static __attribute__((aligned(4))) uint8_t buf[32]; // Buffer for chunk
    static int length = 0;                              // Data in chunk

    buf[length++] = byte;               // Save byte in cunkbuffer
    if (length == sizeof(buf) || force) // Buffer full?
    {
        this->vs1053->playChunk(buf, length); // Send to player
        length = 0;                           // Reset length
    }
    totalByteLength++;
}

uint64_t MediaOutputBuffer::getTotalByteLength()
{
    return this->totalByteLength;
}

void MediaOutputBuffer::clear()
{
    this->totalByteLength = 0;
}
