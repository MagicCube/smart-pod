#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

#define DEBUG true
#define DEBUG_BUFFER_SIZE 100

char *log(const char *format, ...);
char *log(const String message);

#endif
