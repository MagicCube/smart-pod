#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>

#define DEBUG true
#define DEBUG_BUFFER_SIZE 100

char *dbgprint(const char *format, ...);
char *dbgprint(const String format, ...);

#endif
