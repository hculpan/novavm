#include "utils.h"

#include <stdarg.h>
#include <stdio.h>

bool had_errors = false;

void _report_error(char *msg, va_list args)
{
    vfprintf(stdout, msg, args);
}

void report_error(char *msg, ...)
{
    had_errors = true;

    char line_msg[100];
    sprintf(line_msg, "ERROR: %s\n", msg);

    va_list args;
    va_start(args, msg);
    _report_error(line_msg, args);
    va_end(args);
}