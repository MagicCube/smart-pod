#ifndef URL_H
#define URL_H

#include <Arduino.h>

typedef struct
{
    bool isValid = false;
    String protocol;
    String host;
    int port;
    String path;
} URL;

#endif
