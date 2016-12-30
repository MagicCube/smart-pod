#ifndef CONSOLE_H
#define CONSOLE_H

#include <Arduino.h>

#define DEBUG_BUFFER_SIZE 100

enum ConsoleLogLevel
{
    INFO,
    WARN,
    ERROR,
    NONE
};

class Console
{
public:
    static const ConsoleLogLevel logLevel = INFO;
    static char *info(const char *format, ...);
    static char *warn(const char *format, ...);
    static char *error(const char *format, ...);

private:
    static char *appendEntry(ConsoleLogLevel level, char *message);
};

#endif
