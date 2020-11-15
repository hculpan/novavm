#include "bytecode.h"
#include "assembler_config.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

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

bool is_whitespace(char c)
{
    return c == '\t' || c == ' ' || c == '\n' || c == '\r' || c == '\f';
}

char *str_to_upper(char *str)
{
    for (int i = 0; i < strlen(str); i++)
    {
        str[i] = toupper(str[i]);
    }

    return str;
}

bool strempty(char *str)
{
    if (str == NULL)
        return true;

    bool result = true;
    for (int i = 0; i < strlen(str); i++)
    {
        if (!is_whitespace(str[i]))
        {
            result = false;
            break;
        }
    }

    return result;
}

void str_right_trim(char *str)
{
    int i = strlen(str) - 1;
    for (; i >= 0; i--)
    {
        if (is_whitespace(str[i]))
        {
            str[i] = '\0';
        }
        else
        {
            break;
        }
    }
}

void parse_str(char *str, char *tokens[], uint8_t *tokens_length)
{
    char *t = strtok(str, " ,");
    uint8_t index = 0;
    while (t && index < *tokens_length)
    {
        if (strempty(t) || t[0] == '#')
            break;
        str_right_trim(t);
        tokens[index++] = t;
        t = strtok(NULL, " ,");
    }

    *tokens_length = index;
}

Opcode *find_opcode(char *mnemonic)
{
    Opcode *result = NULL;

    for (int i = 0; i < OPCODE_COUNT; i++)
    {
        if (strcmp(mnemonic, OPCODES[i].mnemonic) == 0)
        {
            result = (void *)&OPCODES[i];
            break;
        }
    }

    if (!result)
    {
        report_error("Unrecognized instruction '%s'", mnemonic);
    }

    return result;
}

#define MAX_TOKENS 10
void output_operands(char *tokens[], uint8_t num_tokens, Opcode *opcode, FILE *foutput)
{
    if (!opcode || opcode->operands == 0)
        return;
    if (num_tokens - 1 < opcode->operands)
    {
        report_error("Expected %d operands; found %d", opcode->operands, num_tokens);
        return;
    }

    for (int i = 0; i < opcode->operands; i++)
    {
        int32_t operand = (int32_t)atoi(tokens[i + 1]);
        fwrite(&operand, sizeof(int32_t), 1, foutput);
        code_size += sizeof(int32_t);
    }
}

void assemble_line(char *str, FILE *foutput)
{
    line_no++;

    Opcode *result = NULL;
    char *tokens[MAX_TOKENS];
    uint8_t num_tokens = MAX_TOKENS;

    parse_str(str, tokens, &num_tokens);
    if (num_tokens > 0)
        result = find_opcode(str_to_upper(tokens[0]));

    if (result)
    {
        fwrite(&result->opcode, sizeof(uint8_t), 1, foutput);
        code_size += 1;
        output_operands(tokens, num_tokens, result, foutput);
    }
}

#define MAXCHAR 1000
bool assemble_code(AssemblerConfig *config)
{
    FILE *finput, *foutput;
    char str[MAXCHAR];

    finput = fopen(config->input, "r");
    foutput = fopen(config->output, "wb");
    if (finput == NULL)
    {
        report_error("Could not open input file %s", config->input);
        return false;
    }
    else if (foutput == NULL)
    {
        fclose(finput);
        report_error("Could not open output file %s", config->output);
        return false;
    }
    while (fgets(str, MAXCHAR, finput) != NULL)
    {
        assemble_line(str, foutput);
    }
    fclose(finput);

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
        report_error("Missing required argument: input <file>");
        params_error = true;
    }
    if (!config.output)
    {
        report_error("Missing required argument: output <file>");
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

        if (had_errors)
            puts("Errors reported");
        else
            printf("%d bytes written to %s\n", code_size, config.output);
    }

    free_vm_config(&config);
    exit(0);
}