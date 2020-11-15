#ifndef VM_UTILS_H
#define VM_UTILS_H

#include <stdbool.h>

extern bool had_errors;

void report_error(char *msg, ...);

#endif