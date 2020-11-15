#ifndef ASSEMBLER_UTIL_H
#define ASSEMBLER_UTIL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

extern int line_no;
extern bool had_errors;
extern int32_t code_size;

extern void report_error(char *msg, ...);

#endif