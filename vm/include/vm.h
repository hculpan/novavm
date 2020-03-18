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
    bool verbose_output;
    bool test;
} VmConfig;

bool vm_run(VmConfig *vm_config);

#endif