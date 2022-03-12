#include <stdlib.h>
#include <math.h>
#include "radix_sort.h"

struct {
    int *radix_sort_bits_for_i;
    int *radix_sort_tmp_array;
    int *split_indexes;
    int *split_ldown;
    int *split_lup;
    int *split_reversed_flags;
} radix_global_pre_allocated_heap_arrays;

void permute(const int *input_array, const int *indexes_array, int *output_array, int array_size);
void split(int *input_array, int *flags, int *output_array, int array_size);
void prefix(const int *flags, int *output_array, int array_size);
void suffix(const int *flags, int *output_array, int array_size);
void revert(const int *input_array, int *output_array, int array_size);
void pre_allocate_heap_arrays(int array_size);
void free_heap_arrays();

void radix_sort(int *array, int size, int max_value)
{
    pre_allocate_heap_arrays(size);
    int nb_bits = (int)log2(max_value) + 1;
    for(int i = 0; i < nb_bits; i++)
    {
        for(int j = 0; j < size; j++)
        {
            radix_global_pre_allocated_heap_arrays.radix_sort_bits_for_i[j] = (array[j] >> i) & 1;
        }

        split(array, radix_global_pre_allocated_heap_arrays.radix_sort_bits_for_i, radix_global_pre_allocated_heap_arrays.radix_sort_tmp_array, size);
        for (int j = 0; j < size; ++j)
        {
            array[j] = radix_global_pre_allocated_heap_arrays.radix_sort_tmp_array[j];
        }
    }
    free_heap_arrays();
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
    revert(flags, radix_global_pre_allocated_heap_arrays.split_reversed_flags, array_size);
    prefix(radix_global_pre_allocated_heap_arrays.split_reversed_flags, radix_global_pre_allocated_heap_arrays.split_ldown, array_size);
    suffix(flags, radix_global_pre_allocated_heap_arrays.split_lup, array_size);
    for(int i = 0; i < array_size; i++)
    {
        radix_global_pre_allocated_heap_arrays.split_lup[i] = (array_size + 1) - radix_global_pre_allocated_heap_arrays.split_lup[i];
    }
    for(int i = 0; i < array_size; i++)
    {
        if(flags[i] == 1)
        {
            radix_global_pre_allocated_heap_arrays.split_indexes[i] = radix_global_pre_allocated_heap_arrays.split_lup[i] - 1;
        }
        else
        {
            radix_global_pre_allocated_heap_arrays.split_indexes[i] = radix_global_pre_allocated_heap_arrays.split_ldown[i] - 1;
        }
    }
    permute(input_array, radix_global_pre_allocated_heap_arrays.split_indexes, output_array, array_size);
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

void pre_allocate_heap_arrays(int array_size)
{
    radix_global_pre_allocated_heap_arrays.radix_sort_bits_for_i = (int*)malloc(sizeof(int) * array_size);
    radix_global_pre_allocated_heap_arrays.radix_sort_tmp_array = (int*)malloc(sizeof(int) * array_size);
    radix_global_pre_allocated_heap_arrays.split_indexes = (int*)malloc(sizeof(int) * array_size);
    radix_global_pre_allocated_heap_arrays.split_ldown = (int*)malloc(sizeof(int) * array_size);
    radix_global_pre_allocated_heap_arrays.split_lup = (int*)malloc(sizeof(int) * array_size);
    radix_global_pre_allocated_heap_arrays.split_reversed_flags = (int*)malloc(sizeof(int) * array_size);
}

void free_heap_arrays()
{
    free(radix_global_pre_allocated_heap_arrays.radix_sort_bits_for_i);
    free(radix_global_pre_allocated_heap_arrays.radix_sort_tmp_array);
    free(radix_global_pre_allocated_heap_arrays.split_indexes);
    free(radix_global_pre_allocated_heap_arrays.split_ldown);
    free(radix_global_pre_allocated_heap_arrays.split_lup);
    free(radix_global_pre_allocated_heap_arrays.split_reversed_flags);
}