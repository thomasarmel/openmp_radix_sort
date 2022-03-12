#include <stdlib.h>
#include "config.h"

Config initConfig(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Usage: %s <value of N> <size of the array> [<input file name>] <output file name> [number of threads, otherwise the by default number]\n", argv[0]);
        exit(1);
    }
    Config config;
    config.N = atoi(argv[1]);
    if (config.N <= 0)
    {
        printf("N must be a positive integer\n");
        exit(1);
    }
    config.arraySize = atoi(argv[2]);
    if (config.arraySize <= 0)
    {
        printf("arraySize must be a positive integer\n");
        exit(1);
    }
    int argvOffset = 3;
    config.inputFile = NULL;
    if (argc > 5 || (argc == 5 && atoi(argv[5]) != 0))
    {
        config.inputFile = fopen(argv[3], "r");
        if (config.inputFile == NULL)
        {
            printf("Cannot open input file\n");
            exit(1);
        }
        argvOffset++;
    }
    config.outputFile = fopen(argv[argvOffset], "w");
    if (config.outputFile == NULL)
    {
        printf("Cannot open output file\n");
        exit(1);
    }
    argvOffset++;
    config.nbThreads = -1;
    if (argc > argvOffset)
    {
        config.nbThreads = atoi(argv[argvOffset]);
        if (config.nbThreads <= 0)
        {
            printf("Number of threads must be a positive integer\n");
            exit(1);
        }
    }
    return config;
}

void cleanConfig(Config *config)
{
    if(config->inputFile != NULL)
    {
        fclose(config->inputFile);
    }
    if(config->outputFile != NULL)
    {
        fclose(config->outputFile);
    }
}