#ifndef MEDIA_BUFFER_H
#define MEDIA_BUFFER_H

#include <Arduino.h>

class InputBuffer
{
  public:
    InputBuffer(int bufferSize = 18000);
    bool hasSpace();
    int available();
    void write(uint8_t byte);
    uint8_t read();
    void clear();

  private:
    uint8_t  *bytes;
    uint8_t  size = 0;
    uint16_t writeIndex = 0;
    uint16_t readIndex = 0;
    uint16_t readCount = 0;
};

#endif
