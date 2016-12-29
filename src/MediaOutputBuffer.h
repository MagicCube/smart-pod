#ifndef MEDIA_OUTPUT_BUFFER_H
#define MEDIA_OUTPUT_BUFFER_H

#include <Arduino.h>

class VS1053;

class MediaOutputBuffer
{
  public:
    MediaOutputBuffer(VS1053 *vs1053);
    void write(uint8_t byte, bool force = false);
    uint64_t getTotalByteLength();
    void clear();

  private:
    VS1053 *vs1053;
    uint64_t totalByteLength = 0;
};

#endif
