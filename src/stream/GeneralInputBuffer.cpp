#include "GeneralInputBuffer.h"

GeneralInputBuffer::GeneralInputBuffer(int bufferSize)
{
    size = bufferSize;
    readIndex = size - 1;
    bytes = (uint8_t *)malloc(size);
}

bool GeneralInputBuffer::canWrite()
{
    return readCount < size;
}

int GeneralInputBuffer::available()
{
    return readCount;
}

void GeneralInputBuffer::write(uint8_t byte)
{
    // No check on available space.  See ringspace()
    *(bytes + writeIndex) = byte; // Put byte in ringbuffer
    if (++writeIndex == size)     // Increment pointer and
    {
        writeIndex = 0; // wrap at end
    }
    readCount++; // Count number of bytes in the
}

uint8_t GeneralInputBuffer::read()
{
    // Assume there is always something in the bufferpace.  See ringavail()
    if (++readIndex == size) // Increment pointer and
    {
        readIndex = 0; // wrap at end
    }
    readCount--;                 // Count is now one less
    return *(bytes + readIndex); // return the oldest byte
}

void GeneralInputBuffer::readFromStream(Stream *inputStream, int maxChunkSize)
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

void GeneralInputBuffer::clear()
{
    writeIndex = 0; // Reset ringbuffer administration
    readIndex = size - 1;
    readCount = 0;
}
