#include <stdlib.h>
#include "config.h"

Config init_config(int argc, char **argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "Usage: %s <value of N> <size of the array> [<input file name>] <output file name> [number of threads, otherwise the by default number]\n", argv[0]);
        exit(1);
    }
    Config config;
    config.N = atoi(argv[1]);
    if (config.N <= 0)
    {
        fprintf(stderr, "N must be a positive integer\n");
        exit(1);
    }
    config.array_size = atoi(argv[2]);
    if (config.array_size <= 0)
    {
        fprintf(stderr, "array_size must be a positive integer\n");
        exit(1);
    }
    int argvOffset = 3;
    config.input_file = stdin;
    if (argc > 5 || (argc == 5 && atoi(argv[4]) <= 0))
    {
        config.input_file = fopen(argv[3], "r");
        if (config.input_file == NULL)
        {
            fprintf(stderr, "Cannot open input file\n");
            exit(1);
        }
        argvOffset++;
    }
    config.output_file = fopen(argv[argvOffset], "w");
    if (config.output_file == NULL)
    {
        fprintf(stderr, "Cannot open output file\n");
        exit(1);
    }
    argvOffset++;
    config.nb_threads = -1;
    if (argc > argvOffset)
    {
        config.nb_threads = atoi(argv[argvOffset]);
        if (config.nb_threads <= 0)
        {
            fprintf(stderr, "Number of threads must be a positive integer\n");
            exit(1);
        }
    }
    return config;
}

void clean_config(Config *config)
{
    if(config->input_file != NULL)
    {
        fclose(config->input_file);
    }
    if(config->output_file != NULL)
    {
        fclose(config->output_file);
    }
}