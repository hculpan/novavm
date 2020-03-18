#include "vm-file.h"
#include "vm.h"
#include "bytecode.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

void process_args(int argc, char *argv[], VmConfig *vm_config)
{
    int c;

    while (1)
    {
        static struct option long_options[] =
            {
                {"verbose", no_argument, 0, 'v'},
                {"verbose", no_argument, 0, 't'},
                {"memory", required_argument, 0, 'm'},
                {0, 0, 0, 0}};
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "vtm:",
                        long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
        case 'm':
            vm_config->data_size = atoi(optarg);
            break;

        case 't':
            vm_config->test = true;
            break;

        case 'v':
            vm_config->verbose_output = true;
            break;

        case '?':
            /* getopt_long already printed an error message. */
            break;

        default:
            abort();
        }
    }

    for (int index = optind; index < argc; index++)
    {
        vm_config->filename = strdup(argv[index]);
        break;
    }
}

void free_vm_config(VmConfig *vm_config)
{
    free(vm_config->filename);
}

void load_code(VmConfig *vm_config)
{
    if (vm_config->test)
    {
        puts("Loading test program");
        uint8_t code_tmp[] = {
            OPCODES[ICONST].opcode, 1, 0, 0, 0, // ICONST 1
            OPCODES[ICONST].opcode, 2, 0, 0, 0, // ICONST 2
            OPCODES[IADD].opcode,
            OPCODES[HALT].opcode};
        vm_config->code = malloc(sizeof(code_tmp));
        vm_config->code_size = sizeof(code_tmp);
        memcpy(vm_config->code, code_tmp, sizeof(uint8_t));
    }
}

int main(int argc, char *argv[])
{
    puts("NovaVM v0.0.1");
    VmConfig vm_config = {NULL, 0, NULL, 1000000, false, false};
    process_args(argc, argv, &vm_config);

    if (!vm_config.filename && !vm_config.test)
    {
        printf("Missing NVM file\n");
        exit(0);
    }
    else
    {
        load_code(&vm_config);
    }

    if (vm_config.verbose_output)
    {
        puts("  Verbose output enabled");
        printf("  Input file: %s\n", vm_config.filename);
        printf("  Code size: %d\n", vm_config.code_size);
        printf("  Data size: %d\n", vm_config.data_size);
    }

    vm_run(&vm_config);

    free_vm_config(&vm_config);

    exit(0);
}