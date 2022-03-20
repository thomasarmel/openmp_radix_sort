#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "radix_sort.h"
#ifdef USE_PAR_PRE_SUF
#include "parallel_prefix_suffix.h"
#endif // USE_PAR_PRE_SUF

struct {
    int *radix_sort_bits_for_i;
    int *radix_sort_tmp_array;
    int *split_indexes;
    int *split_ldown;
    int *split_lup;
    int *split_reversed_flags;
} radix_global_pre_allocated_heap_arrays; // Preallocate heap arrays for radix sort (avoid costly malloc/free)

void permute(const int *input_array, const int *indexes_array, int *output_array, int array_size);
void split(int *input_array, int *flags, int *output_array, int array_size);
void prefix(const int *flags, int *output_array, int array_size);
void suffix(const int *flags, int *output_array, int array_size);
void revert(const int *input_array, int *output_array, int array_size);
void pre_allocate_heap_arrays(int array_size);
void free_heap_arrays();

void radix_sort(int *array, int size, int max_value)
{
    pre_allocate_heap_arrays(size); // Preallocate heap arrays for radix sort (avoid costly malloc/free)
    int nb_bits = (int)log2(max_value) + 1;
    for(int i = 0; i < nb_bits; i++)
    {
        #pragma omp parallel for
        for(int j = 0; j < size; j++) // bit(i)
        {
            radix_global_pre_allocated_heap_arrays.radix_sort_bits_for_i[j] = (array[j] >> i) & 1;
        }

        split(array, radix_global_pre_allocated_heap_arrays.radix_sort_bits_for_i, radix_global_pre_allocated_heap_arrays.radix_sort_tmp_array, size);
        memcpy(array, radix_global_pre_allocated_heap_arrays.radix_sort_tmp_array, size * sizeof(int));
    }
    free_heap_arrays(); // Free heap arrays for radix sort (avoid costly malloc/free)
}

// Input = [5, 7, 3, 1, 4, 2, 7, 2], Indexes = [3, 4, 5, 6, 0, 1, 7, 2], Output = [4, 2, 2, 5, 7, 3, 1, 7]
void permute(const int *input_array, const int *indexes_array, int *output_array, int array_size)
{
    #pragma omp parallel for
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
    #pragma omp parallel for
    for(int i = 0; i < array_size; i++) // Iup = SIZE - SUFFIX (Flags);
    {
        radix_global_pre_allocated_heap_arrays.split_lup[i] = (array_size + 1) - radix_global_pre_allocated_heap_arrays.split_lup[i];
    }
    #pragma omp parallel for
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

// Prefix sum, use naive iterative version if USE_PAR_PRE_SUF is not defined
void prefix(const int *flags, int *output_array, int array_size)
{
#ifdef USE_PAR_PRE_SUF
    memcpy(output_array, flags, array_size * sizeof(int));
    parallel_prefix(output_array, array_size);
#else
    output_array[0] = flags[0];
    for(int i = 1; i < array_size; i++)
    {
        output_array[i] = output_array[i - 1] + flags[i];
    }
#endif // USE_PAR_PRE_SUF
}

// Suffix sum, use naive iterative version if USE_PAR_PRE_SUF is not defined
void suffix(const int *flags, int *output_array, int array_size)
{
#ifdef USE_PAR_PRE_SUF
    memcpy(output_array, flags, array_size * sizeof(int));
    parallel_suffix(output_array, array_size);
#else
    output_array[array_size - 1] = flags[array_size - 1];
    for(int i = array_size - 2; i >= 0; i--)
    {
        output_array[i] = output_array[i + 1] + flags[i];
    }
#endif // USE_PAR_PRE_SUF
}

// Revert boolean array
void revert(const int *input_array, int *output_array, int array_size)
{
    #pragma omp parallel for
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