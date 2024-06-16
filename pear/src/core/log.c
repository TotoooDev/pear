#include <core/log.h>
#include <stdarg.h>
#include <stdio.h>

void pear_log(const char* message, ...) {
    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);
}
