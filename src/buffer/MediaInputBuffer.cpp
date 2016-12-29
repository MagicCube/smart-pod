#include "MediaInputBuffer.h"

MediaInputBuffer::MediaInputBuffer(int bufferSize)
{
    size = bufferSize;
    readIndex = size - 1;
    bytes = (uint8_t *)malloc(size);
}

bool MediaInputBuffer::canWrite()
{
    return readCount < size;
}

int MediaInputBuffer::available()
{
    return readCount;
}

void MediaInputBuffer::write(uint8_t byte)
{
    // No check on available space.  See ringspace()
    *(bytes + writeIndex) = byte; // Put byte in ringbuffer
    if (++writeIndex == size)     // Increment pointer and
    {
        writeIndex = 0; // wrap at end
    }
    readCount++; // Count number of bytes in the
}

uint8_t MediaInputBuffer::read()
{
    // Assume there is always something in the bufferpace.  See ringavail()
    if (++readIndex == size) // Increment pointer and
    {
        readIndex = 0; // wrap at end
    }
    readCount--;                 // Count is now one less
    return *(bytes + readIndex); // return the oldest byte
}

void MediaInputBuffer::readFromStream(Stream *inputStream, int maxChunkSize)
{
    int length;
    length = inputStream->available();
    if (length > maxChunkSize) // Reduce byte count for this loop()
    {
        length = maxChunkSize;
    }

    while (this->canWrite() && length--)
    {
        this->write(inputStream->read()); // Yes, store one byte in inputBuffer
    }
}

void MediaInputBuffer::clear()
{
    writeIndex = 0; // Reset ringbuffer administration
    readIndex = size - 1;
    readCount = 0;
}
