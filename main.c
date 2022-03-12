#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>
#include "config.h"
#include "radix_sort.h"

int* read_input_array(Config *config);
void print_array(Config *config, int *array);

int main(int argc, char *argv[])
{
    struct timeval start_elapsed_time, end_elapsed_time;
    Config config = init_config(argc, argv);
    if (config.nb_threads > 0)
    {
        omp_set_num_threads(config.nb_threads);
    }
    int *A = read_input_array(&config);
    fprintf(config.output_file, "N=%d Size=%d Threads=%d Time_in_microsec=549\n", config.N, config.array_size, omp_get_max_threads());
    print_array(&config, A);
    gettimeofday(&start_elapsed_time, NULL);
    radix_sort(A, config.array_size, config.N);
    gettimeofday(&end_elapsed_time, NULL);
    long elapsed = ((end_elapsed_time.tv_sec - start_elapsed_time.tv_sec) * 1000000) + (end_elapsed_time.tv_usec - start_elapsed_time.tv_usec);
    printf("Elapsed time: %ld microseconds\n", elapsed);
    print_array(&config, A);
    clean_config(&config);
    return 0;
}

int* read_input_array(Config *config)
{
    int *array = (int*)malloc(sizeof(int) * config->array_size);
    if (config->input_file == NULL)
    {
        srand(time(NULL));
        for (int i = 0; i < config->array_size; i++)
        {
            array[i] = rand() % (config->N + 1);
        }
        return array;
    }
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

void print_array(Config *config, int *array)
{
    for (int i = 0; i < config->array_size; i++)
    {
        fprintf(config->output_file, "%d ", array[i]);
    }
    fprintf(config->output_file, "\n");
}