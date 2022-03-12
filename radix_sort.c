#include <stdlib.h>
#include <math.h>
#include "radix_sort.h"

void permute(const int *input_array, const int *indexes_array, int *output_array, int array_size);
void split(int *input_array, int *flags, int *output_array, int array_size);
void prefix(const int *flags, int *output_array, int array_size);
void suffix(const int *flags, int *output_array, int array_size);
void revert(const int *input_array, int *output_array, int array_size);

void radix_sort(int *array, int size, int max_value)
{
    int nb_bits = (int)log2(max_value) + 1;
    for(int i = 0; i < nb_bits; i++)
    {
        int *bits_for_i = (int*)malloc(sizeof(int) * size);
        for(int j = 0; j < size; j++)
        {
            bits_for_i[j] = (array[j] >> i) & 1;
        }
        int *tmp_array = (int*)malloc(sizeof(int) * size);
        split(array, bits_for_i, tmp_array, size);
        for (int j = 0; j < size; ++j)
        {
            array[j] = tmp_array[j];
        }
        free(bits_for_i);
        free(tmp_array);
    }
}

void permute(const int *input_array, const int *indexes_array, int *output_array, int array_size)
{
    for(int i = 0; i < array_size; i++)
    {
        output_array[indexes_array[i]] = input_array[i];
    }
}

void split(int *input_array, int *flags, int *output_array, int array_size)
{
    int *indexes = (int*)malloc(sizeof(int) * array_size);
    int *ldown = (int*)malloc(sizeof(int) * array_size);
    int *lup = (int*)malloc(sizeof(int) * array_size);
    int *reversed_flags = (int*)malloc(sizeof(int) * array_size);
    revert(flags, reversed_flags, array_size);
    prefix(reversed_flags, ldown, array_size);
    suffix(flags, lup, array_size);
    for(int i = 0; i < array_size; i++)
    {
        lup[i] = (array_size + 1) - lup[i];
    }
    free(reversed_flags);
    for(int i = 0; i < array_size; i++)
    {
        if(flags[i] == 1)
        {
            indexes[i] = lup[i] - 1;
        }
        else
        {
            indexes[i] = ldown[i] - 1;
        }
    }
    permute(input_array, indexes, output_array, array_size);
    free(indexes);
    free(ldown);
    free(lup);
}

void prefix(const int *flags, int *output_array, int array_size)
{
    output_array[0] = flags[0];
    for(int i = 1; i < array_size; i++)
    {
        output_array[i] = output_array[i - 1] + flags[i];
    }
}

void suffix(const int *flags, int *output_array, int array_size)
{
    output_array[array_size - 1] = flags[array_size - 1];
    for(int i = array_size - 2; i >= 0; i--)
    {
        output_array[i] = output_array[i + 1] + flags[i];
    }
}

void revert(const int *input_array, int *output_array, int array_size)
{
    for(int i = 0; i < array_size; i++)
    {
        output_array[i] = !input_array[i];
    }
}