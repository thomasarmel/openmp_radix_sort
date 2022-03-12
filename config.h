#ifndef RADIX_SORT_CONFIG_H
#define RADIX_SORT_CONFIG_H

#include <stdio.h>

struct config
{
    int N;
    int array_size;
    FILE *input_file;
    FILE *output_file;
    int nb_threads;
};
typedef struct config Config;

Config init_config(int argc, char **argv);
void clean_config(Config *config);

#endif //RADIX_SORT_CONFIG_H
