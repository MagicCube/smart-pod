#ifndef INPUT_STREAM_BUFFER_H
#define INPUT_STREAM_BUFFER_H

#include <Arduino.h>

class GeneralInputBuffer
{
  public:
    GeneralInputBuffer(int bufferSize = 18000);
    bool canWrite();
    int available();
    void write(uint8_t byte);
    uint8_t read();
    void readFromStream(Stream* inputStream, int maxChunkSize = 1024);
    void clear();

  private:
    uint8_t  *bytes;
    uint8_t  size = 0;
    uint16_t writeIndex = 0;
    uint16_t readIndex = 0;
    uint16_t readCount = 0;
};

#endif