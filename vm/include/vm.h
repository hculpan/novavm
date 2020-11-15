#ifndef VM_H
#define VM_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    char *filename;
    uint32_t code_size;
    uint8_t *code;
    uint32_t data_size;
    uint8_t *data;
    int32_t *stack;
    bool verbose_output;
    bool test;

    // registers
    uint32_t ip;
    uint32_t sp;
} VmConfig;

bool vm_run(VmConfig *vm_config);

VmConfig *create_config();

#endif