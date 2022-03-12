#ifndef RADIX_SORT_CONFIG_H
#define RADIX_SORT_CONFIG_H

#include <stdio.h>

struct _config
{
    int N;
    int arraySize;
    FILE *inputFile;
    FILE *outputFile;
    int nbThreads;
};
typedef struct _config Config;

Config initConfig(int argc, char **argv);
void cleanConfig(Config *config);

#endif //RADIX_SORT_CONFIG_H
