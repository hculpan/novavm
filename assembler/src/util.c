#include "util.h"

#include <stdio.h>

int line_no = 0;
bool had_errors = false;
int32_t code_size = 0;

void _report_error(char *msg, va_list args)
{
    vfprintf(stdout, msg, args);
}

void report_error(char *msg, ...)
{
    had_errors = true;

    char line_msg[100];
    sprintf(line_msg, "Error [%04d]: %s\n", line_no, msg);

    va_list args;
    va_start(args, msg);
    _report_error(line_msg, args);
    va_end(args);
}
