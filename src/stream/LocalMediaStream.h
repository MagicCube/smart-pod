#ifndef LOCAL_MEDIA_STREAM
#define LOCAL_MEDIA_STREAM

#include <Arduino.h>

#include <FS.h>

#include "MediaStream.h"

class LocalMediaStream : public MediaStream
{
  public:
    LocalMediaStream(String path);
    int available() override;
    int read() override;
    int peek() override;
    void flush() override;

    int totalSize() override;

  private:
    File _fileStream;
};

#endif