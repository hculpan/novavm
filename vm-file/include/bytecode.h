#ifndef BYTECODE_H
#define BYTECODE_H

#include <stdint.h>

#define HALT 0
#define ICONST 1
#define IADD 2
#define ISUB 3
#define IMUL 4
#define IDIV 5
#define IPRINT 6
#define JMP 7
#define ISTORE 8
#define ILOAD 9
#define IMOD 10

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

#define ISUB_O          \
    {                   \
        ISUB, 0, "ISUB" \
    }

#define IMUL_O          \
    {                   \
        IMUL, 0, "IMUL" \
    }

#define IDIV_O          \
    {                   \
        IDIV, 0, "IDIV" \
    }

#define IPRINT_O            \
    {                       \
        IPRINT, 0, "IPRINT" \
    }

#define JMP_O         \
    {                 \
        JMP, 1, "JMP" \
    }

#define ISTORE_O            \
    {                       \
        ISTORE, 1, "ISTORE" \
    }

#define ILOAD_O           \
    {                     \
        ILOAD, 1, "ILOAD" \
    }

#define IMOD_O          \
    {                   \
        IMOD, 0, "IMOD" \
    }

const Opcode OPCODES[] = {HALT_O, ICONST_O, IADD_O, ISUB_O, IMUL_O, IDIV_O, IPRINT_O, JMP_O, ISTORE_O, ILOAD_O, IMOD_O};

const uint8_t OPCODE_COUNT = sizeof(OPCODES) / sizeof(Opcode);

#endif