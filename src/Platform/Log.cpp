#include "Platform/Log.h"

void logMsg(const char * format, ...)
{
   va_list args;
   va_start(args, format);
   vprintf(format, args);
   va_end(args);
   printf("\n");
}