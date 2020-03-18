#ifndef BYTECODE_H
#define BYTECODE_H

#include <stdint.h>

#define HALT 0
#define ICONST 1
#define IADD 2
#define IPRINT 3

typedef struct
{
    uint8_t opcode;
    uint8_t operands;
    char *mnemonic;
} Opcode;

#define HALT_O          \
    {                   \
        HALT, 0, "HALT" \
    }

#define ICONST_O            \
    {                       \
        ICONST, 1, "ICONST" \
    }

#define IADD_O          \
    {                   \
        IADD, 0, "IADD" \
    }

#define IPRINT_O            \
    {                       \
        IPRINT, 0, "IPRINT" \
    }

const Opcode OPCODES[] = {HALT_O, ICONST_O, IADD_O, IPRINT_O};

#endif