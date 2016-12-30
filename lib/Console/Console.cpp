#include "Console.h"

char *Console::info(const char *format, ...)
{
    static char sbuf[DEBUG_BUFFER_SIZE];
    va_list varArgs;
    va_start(varArgs, format);
    vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
    va_end(varArgs);

    if (logLevel <= INFO)
    {
        Serial.print("<i> ");
        Serial.println(sbuf);
    }
    return sbuf;
}

char *Console::warn(const char *format, ...)
{
    static char sbuf[DEBUG_BUFFER_SIZE];
    va_list varArgs;
    va_start(varArgs, format);
    vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
    va_end(varArgs);

    if (logLevel <= WARN)
    {
        Serial.print("<W> ");
        Serial.println(sbuf);
    }
    return sbuf;
}

char *Console::error(const char *format, ...)
{
    static char sbuf[DEBUG_BUFFER_SIZE];
    va_list varArgs;
    va_start(varArgs, format);
    vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
    va_end(varArgs);

    if (logLevel <= ERROR)
    {
        Serial.print("<E> ");
        Serial.println(sbuf);
    }
    return sbuf;
}
