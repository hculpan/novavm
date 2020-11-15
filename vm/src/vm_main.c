#include "vm-file.h"
#include "vm.h"
#include "utils.h"

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
    free(vm_config->code);
    free(vm_config->data);
    free(vm_config);
}

void load_code_and_data(VmConfig *vm_config)
{
    FILE *finput;
    finput = fopen(vm_config->filename, "rb");
    if (!finput)
    {
        report_error("Unable to open file %s", vm_config->filename);
        return;
    }
    fseek(finput, 0l, SEEK_END);
    vm_config->code_size = ftell(finput);
    vm_config->code = malloc(vm_config->code_size);
    fseek(finput, 0l, SEEK_SET);
    fread(vm_config->code, sizeof(uint8_t), vm_config->code_size, finput);
    fclose(finput);

    vm_config->data = malloc(vm_config->data_size);
}

int main(int argc, char *argv[])
{
    puts("NovaVM v0.0.1");
    VmConfig *vm_config = create_config();
    process_args(argc, argv, vm_config);

    if (!vm_config->filename && !vm_config->test)
    {
        report_error("Missing NVM file");
        exit(0);
    }
    else
    {
        load_code_and_data(vm_config);
    }

    if (had_errors)
        exit(0);

    if (!vm_run(vm_config))
        puts("Exited with errors");

    free_vm_config(vm_config);

    exit(0);
}