#include "Console.h"

void Console::begin(int baudRate)
{
    Serial.begin(baudRate);
    Serial.println("\n\n");
}

char *Console::appendEntry(ConsoleLogLevel level, char *message)
{
    if (level >= Console::logLevel)
    {
        switch (level)
        {
            case DEBUG:
                Serial.print("[DBG]");
                Serial.print(" ");
                Serial.println(message);
                break;
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
            case FATAL:
                Serial.println();
                Serial.println("=============================== [FATAL] ===============================");
                Serial.println(message);
                Serial.println();
                break;
        }
    }
    return message;
}



void Console::line()
{
    if (INFO >= Console::logLevel)
    {
        Serial.println("--------------------------------------------------------------------------------");
    }
}

char *Console::debug(const char *format, ...)
{
    static char sbuf[DEBUG_BUFFER_SIZE];
    va_list varArgs;
    va_start(varArgs, format);
    vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
    va_end(varArgs);
    return appendEntry(DEBUG, sbuf);
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

char *Console::fatal(const char *format, ...)
{
    static char sbuf[DEBUG_BUFFER_SIZE];
    va_list varArgs;
    va_start(varArgs, format);
    vsnprintf(sbuf, sizeof(sbuf), format, varArgs);
    va_end(varArgs);
    return appendEntry(FATAL, sbuf);
}
