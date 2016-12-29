#include "InputBuffer.h"

InputBuffer::InputBuffer(int bufferSize)
{
    size = bufferSize;
    readIndex = size - 1;
    bytes = (uint8_t *)malloc(size);
}

bool InputBuffer::hasSpace()
{
    return readCount < size;
}

int InputBuffer::available()
{
    return readCount;
}

void InputBuffer::write(uint8_t byte)
{
    // No check on available space.  See ringspace()
    *(bytes + writeIndex) = byte; // Put byte in ringbuffer
    if (++writeIndex == size)     // Increment pointer and
    {
        writeIndex = 0; // wrap at end
    }
    readCount++; // Count number of bytes in the
}

uint8_t InputBuffer::read()
{
    // Assume there is always something in the bufferpace.  See ringavail()
    if (++readIndex == size) // Increment pointer and
    {
        readIndex = 0; // wrap at end
    }
    readCount--;                 // Count is now one less
    return *(bytes + readIndex); // return the oldest byte
}

void InputBuffer::clear()
{
    writeIndex = 0; // Reset ringbuffer administration
    readIndex = size - 1;
    readCount = 0;
}
