#ifndef CONSOLE_H
#define CONSOLE_H

#include <Arduino.h>

#define DEBUG_BUFFER_SIZE 100

enum ConsoleLogLevel
{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NONE
};

class Console
{
public:
    static const ConsoleLogLevel logLevel = DEBUG;

    static void begin(int baudRate = 115200);
    static char *debug(const char *format, ...);
    static char *info(const char *format, ...);
    static char *warn(const char *format, ...);
    static char *error(const char *format, ...);
    static char *fatal(const char *format, ...);
    static void line();

private:
    static char *appendEntry(ConsoleLogLevel level, char *message);
};

#endif
