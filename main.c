#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "radix_sort.h"

int* read_input_array(Config *config);

int main(int argc, char *argv[])
{
    Config config = init_config(argc, argv);
    int *A = read_input_array(&config);
    radix_sort(A, config.array_size, config.N);
    for(int i = 0; i < config.array_size; i++)
    {
        fprintf(config.output_file, "%d ", A[i]);
    }
    fprintf(config.output_file, "\n");
    clean_config(&config);
    return 0;
}

int* read_input_array(Config *config)
{
    int *array = (int*)malloc(sizeof(int) * config->array_size);
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, config->input_file) == -1)
    {
        fprintf(stderr, "Error: Could not read input file\n");
        exit(1);
    }
    char *token = strtok(line, " ");
    int i = 0;
    while (token != NULL)
    {
        array[i] = atoi(token);
        token = strtok(NULL, " ");
        i++;
    }
    return array;
}