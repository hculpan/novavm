#include "vm.h"
#include "bytecode.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t opcode = 0;
int32_t operands[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void fetch(VmConfig *vm_config);
void display_stack(VmConfig *vm_config);

void push(VmConfig *vm_config, int32_t operand);
int32_t pop(VmConfig *vm_config);

void iprint(VmConfig *vm_config);
void iadd(VmConfig *vm_config);
void isub(VmConfig *vm_config);
void imul(VmConfig *vm_config);
void idiv(VmConfig *vm_config);
void jmp(VmConfig *vm_config);
void istore(VmConfig *vm_config);
void iload(VmConfig *vm_config);
void imod(VmConfig *vm_config);

bool vm_run(VmConfig *vm_config)
{
    if (vm_config->verbose_output)
    {
        puts("  Verbose output enabled");
        printf("  Input file: %s\n", vm_config->filename);
        printf("  Code size: %d\n", vm_config->code_size);
        printf("  Data size: %d\n", vm_config->data_size);
    }

    fetch(vm_config);
    if (had_errors)
        return false;

    while (vm_config->ip <= vm_config->code_size)
    {
        switch (opcode)
        {
        case HALT:
            goto done;

        case ICONST:
            push(vm_config, operands[0]);
            break;

        case IPRINT:
            iprint(vm_config);
            break;

        case IADD:
            iadd(vm_config);
            break;

        case ISUB:
            isub(vm_config);
            break;

        case IMUL:
            imul(vm_config);
            break;

        case IDIV:
            idiv(vm_config);
            break;

        case JMP:
            jmp(vm_config);
            break;

        case ISTORE:
            istore(vm_config);
            break;

        case ILOAD:
            iload(vm_config);
            break;

        case IMOD:
            imod(vm_config);
            break;

        default:
            break;
        }
        if (had_errors)
            return false;

        if (vm_config->test)
            display_stack(vm_config);

        fetch(vm_config);
        if (had_errors)
            return false;
    }

done:

    return true;
}

void istore(VmConfig *vm_config)
{
    if (operands[0] < 0 || operands[0] >= vm_config->data_size)
    {
        report_error("Invalid data location for ISTORE: %d", operands[0]);
        return;
    }

    int32_t v = pop(vm_config);
    if (had_errors)
        return;

    int32_t *address = (int32_t *)&vm_config->data[operands[0]];
    *address = v;
}

void iload(VmConfig *vm_config)
{
    if (operands[0] < 0 || operands[0] >= vm_config->data_size)
    {
        report_error("Invalid data location for ILOAD: %d", operands[0]);
        return;
    }

    int32_t *address = (int32_t *)&vm_config->data[operands[0]];
    push(vm_config, *address);
}

void jmp(VmConfig *vm_config)
{
    vm_config->ip = operands[0];
}

void imul(VmConfig *vm_config)
{
    int32_t v1 = pop(vm_config);
    int32_t v2 = pop(vm_config);
    if (had_errors)
        return;

    push(vm_config, v2 * v1);
}

void imod(VmConfig *vm_config)
{
    int32_t v1 = pop(vm_config);
    int32_t v2 = pop(vm_config);
    if (had_errors)
        return;

    push(vm_config, (int32_t)(v2 % v1));
}

void idiv(VmConfig *vm_config)
{
    int32_t v1 = pop(vm_config);
    int32_t v2 = pop(vm_config);
    if (had_errors)
        return;

    push(vm_config, (int32_t)(v2 / v1));
}

void isub(VmConfig *vm_config)
{
    int32_t v1 = pop(vm_config);
    int32_t v2 = pop(vm_config);
    if (had_errors)
        return;

    push(vm_config, v2 - v1);
}

void iadd(VmConfig *vm_config)
{
    int32_t v1 = pop(vm_config);
    int32_t v2 = pop(vm_config);
    if (had_errors)
        return;

    push(vm_config, v1 + v2);
}

void iprint(VmConfig *vm_config)
{
    int32_t v = pop(vm_config);
    if (had_errors)
        return;

    printf("%d\n", v);
}

void display_stack_value(VmConfig *vm_config, uint32_t sp)
{
    int32_t *data = (int32_t *)&vm_config->data[sp];
    printf(" [%d] %04d", sp, *data);
}

void display_stack(VmConfig *vm_config)
{
    uint32_t temp_sp = vm_config->sp;
    printf("Stack [%d]: ", vm_config->sp);
    for (int i = 0; i < 5; i++)
    {
        if (temp_sp > vm_config->data_size)
            break;

        display_stack_value(vm_config, temp_sp);
        temp_sp += 4;
    }

    printf("\n");
}

void push(VmConfig *vm_config, int32_t operand)
{
    vm_config->sp -= 4;
    if (vm_config->sp < 0)
    {
        vm_config->sp += 4;
        report_error("Stack overflow!");
        return;
    }

    int32_t *d = (int32_t *)&vm_config->data[vm_config->sp];
    *d = operand;
}

int32_t pop(VmConfig *vm_config)
{

    if (vm_config->sp + 4 >= vm_config->data_size)
    {
        report_error("Stack underflow!");
        return 0;
    }

    int32_t *d = (int32_t *)&vm_config->data[vm_config->sp];
    vm_config->sp += 4;
    return *d;
}

void fetch(VmConfig *vm_config)
{
    opcode = vm_config->code[vm_config->ip++];
    if (opcode >= OPCODE_COUNT)
    {
        report_error("Invalid instruction: %d", opcode);
        return;
    }

    for (int i = 0; i < OPCODES[opcode].operands; i++)
    {
        if (sizeof(uint32_t) + vm_config->ip > vm_config->code_size)
        {
            report_error("Operand size exceeds code segment");
            return;
        }

        int32_t operand = *((int32_t *)&vm_config->code[vm_config->ip]);
        vm_config->ip += 4;
        operands[i] = operand;
    }
}

VmConfig *create_config()
{
    VmConfig *result = malloc(sizeof(VmConfig));
    result->filename = NULL;
    result->code_size = 0;
    result->code = NULL;
    result->data_size = 1000;
    result->data = NULL;
    result->verbose_output = false;
    result->test = false;
    result->ip = 0;
    result->sp = result->data_size - 1;
    return result;
}