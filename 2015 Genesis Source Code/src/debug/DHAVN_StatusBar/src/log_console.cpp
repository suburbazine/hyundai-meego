#include "log_console.h"

void lprintf(const char* fmt, ...) {


    va_list args;
    char msg[8192];


    va_start(args, fmt);
    vsnprintf(msg, 8192, fmt, args);
    va_end(args);

    Logger:: Instance()->Log(msg, Logger::PRIORITY_LOW);
//    Logger:: Instance()->Log(msg, Logger::PRIORITY_HI);
}

