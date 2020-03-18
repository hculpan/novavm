#ifndef ASSEMBLER_CONFIG_H
#define ASSEMBLER_CONFIG_H

#include <stdbool.h>

typedef struct
{
    char *input;
    char *output;
    bool verbose;
} AssemblerConfig;

#endif