#include "bytecode.h"
#include "assembler_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>

void process_args(int argc, char *argv[], AssemblerConfig *config)
{
    int c;

    while (1)
    {
        static struct option long_options[] =
            {
                {"verbose", no_argument, 0, 'v'},
                {"test", no_argument, 0, 't'},
                {"input", required_argument, 0, 'i'},
                {"output", required_argument, 0, 'o'},
                {0, 0, 0, 0}};
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long(argc, argv, "vti:o:",
                        long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c)
        {
        case 'i':
            config->input = strdup(optarg);
            break;

        case 'o':
            config->output = strdup(optarg);
            break;

        case 'v':
            config->verbose = true;
            break;

        case '?':
            /* getopt_long already printed an error message. */
            break;

        default:
            abort();
        }
    }
}

void free_vm_config(AssemblerConfig *config)
{
    free(config->input);
    free(config->output);
}

/*void load_code(VmConfig *vm_config)
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
}*/

bool assemble_code(AssemblerConfig *config)
{
    return true;
}

int main(int argc, char *argv[])
{
    puts("Nova Assembler v0.0.1");
    AssemblerConfig config = {NULL, NULL, false};
    process_args(argc, argv, &config);

    bool params_error = false;
    if (!config.input)
    {
        printf("Missing required argument: input <file>\n");
        params_error = true;
    }
    if (!config.output)
    {
        printf("Missing required argument: output <file>\n");
        params_error = true;
    }

    if (!params_error)
    {
        if (config.verbose)
        {
            puts("  Verbose output enabled");
            printf("  Input file: %s\n", config.input);
            printf("  Output file: %s\n", config.output);
        }

        assemble_code(&config);
    }

    free_vm_config(&config);
    exit(0);
}