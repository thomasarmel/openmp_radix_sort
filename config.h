#ifndef RADIX_SORT_CONFIG_H
#define RADIX_SORT_CONFIG_H

#include <stdio.h>

struct config
{
    int N; // Max number
    int array_size; // Size of array
    FILE *input_file; // Input file descriptor
    FILE *output_file; // Output file descriptor (random numbers if not specified)
    int nb_threads; // Number of threads, if specified
};
typedef struct config Config;

Config init_config(int argc, char **argv);
void clean_config(Config *config);

#endif //RADIX_SORT_CONFIG_H
