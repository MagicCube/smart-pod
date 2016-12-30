#ifndef URL_H
#define URL_H

#include <Arduino.h>

struct URL
{
    bool isValid = false;
    String protocol;
    String host;
    int port;
    String path;
};

#endif
