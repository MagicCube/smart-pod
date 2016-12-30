#include "Console.h"

char *Console::appendEntry(ConsoleLogLevel level, char *message)
{
    if (level >= Console::logLevel)
    {
        switch (level)
        {
            case INFO:
                Serial.print("[INF]");
                Serial.print(" ");
                Serial.println(message);
                break;
            case WARN:
                Serial.print("[WRN]");
                Serial.print(" ** ");
                Serial.print(message);
                Serial.println(" ** ");
                break;
            case ERROR:
                Serial.print("[ERR]");
                Serial.print(" >> ");
                Serial.print(message);
                Serial.println(" << ");
                break;
        }
    }
    return message;
}

char *Console::info(const char *format, ...)
{
    static char sbuf[DEBUG_BUFFER_SIZE];
    va_list varArgs;
    va_start(varArgs, format);
    vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
    va_end(varArgs);

    return appendEntry(INFO, sbuf);
}

char *Console::warn(const char *format, ...)
{
    static char sbuf[DEBUG_BUFFER_SIZE];
    va_list varArgs;
    va_start(varArgs, format);
    vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
    va_end(varArgs);

    return appendEntry(WARN, sbuf);
}

char *Console::error(const char *format, ...)
{
    static char sbuf[DEBUG_BUFFER_SIZE];
    va_list varArgs;
    va_start(varArgs, format);
    vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
    va_end(varArgs);

    return appendEntry(ERROR, sbuf);
}
