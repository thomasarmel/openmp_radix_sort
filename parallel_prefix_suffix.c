#include <stdlib.h>
#include <math.h>
#include "parallel_prefix_suffix.h"

struct tablo
{
    int *tab; // Pointer to the heap-stored array of integers
    int size; // Size of the array (2 times input array)
};

struct tablo *allocate_tablo(int size);
void free_tablo(struct tablo *tablo);
void parallel_prefix_sum(struct tablo *tablo);
void parallel_suffix_sum(struct tablo *tablo);

void parallel_prefix(int *tab, int size)
{
    int max_log_size = (int)log2(size);
    int previous_max_prefix = 0;
    int offset = 0;
    // Split into sub arrays of size 2^i (eg: 13 -> 8, 4, 1)
    for(int i = max_log_size; i >= 0; i--)
    {
        int real_subarray_size = 1 << i; // pow(2, i)
        if (size < offset + real_subarray_size) // 13 -> 8, 4, 1, skip array of size 2
        {
            continue;
        }
        struct tablo *sub_tablo = allocate_tablo(real_subarray_size * 2);
#pragma omp parallel for
        for(int j = 0; j < real_subarray_size; j++) // Copy input
        {
            sub_tablo->tab[j + real_subarray_size] = tab[j + offset];
        }
        parallel_prefix_sum(sub_tablo); // Apply parallel prefix sum for sub array of size 2^i
#pragma omp parallel for
        for(int j = 0; j < real_subarray_size; j++) // Copy output
        {
            tab[j + offset] = sub_tablo->tab[j + real_subarray_size] + previous_max_prefix;
        }
        previous_max_prefix += sub_tablo->tab[(2 * real_subarray_size) - 1];
        free_tablo(sub_tablo);
        offset += real_subarray_size;
    }
}

void parallel_suffix(int *tab, int size)
{
    int max_log_size = log2(size);
    int previous_max_suffix = 0;
    int offset = 0;
    // Split into sub arrays of size 2^i (eg: 13 -> 8, 4, 1)
    for(int i = max_log_size; i >= 0; i--)
    {
        int real_subarray_size = 1 << i; // pow(2, i)
        if (size < offset + real_subarray_size)
        {
            continue;
        }
        struct tablo *sub_tablo = allocate_tablo(real_subarray_size * 2);
#pragma omp parallel for
        for(int j = real_subarray_size - 1; j >= 0; j--) // Copy input
        {
            sub_tablo->tab[j + real_subarray_size] = tab[size - offset - real_subarray_size + j];
        }
        parallel_suffix_sum(sub_tablo); // Apply parallel suffix sum for sub array of size 2^i
#pragma omp parallel for
        for(int j = 0; j < real_subarray_size; j++) // Copy output
        {
            tab[size - offset - real_subarray_size + j] = sub_tablo->tab[j + real_subarray_size] + previous_max_suffix;
        }
        previous_max_suffix += sub_tablo->tab[real_subarray_size];
        free_tablo(sub_tablo);
        offset += real_subarray_size;
    }
}

struct tablo *allocate_tablo(int size)
{
    struct tablo *tmp = malloc(sizeof(struct tablo));
    tmp->size = size;
    tmp->tab = malloc(size*sizeof(int));
    return tmp;
}

void free_tablo(struct tablo *tablo)
{
    free(tablo->tab);
    free(tablo);
}

void parallel_prefix_sum(struct tablo *tablo)
{
    int *old_tab = (int*) malloc((tablo->size / 2) * sizeof(int));
#pragma omp parallel for
    for(int i=0; i<tablo->size / 2; i++) // Copy input in a way to sum it to the output
    {
        old_tab[i] = tablo->tab[i + (tablo->size / 2)];
    }
    int nbSteps = (int)log2(tablo->size / 2) - 1;
    // ascendant step
    for(int l = nbSteps; l>=0; l--)
    {
        const int step_max = 1 << (l + 1);
#pragma omp parallel for
        for(int j = 1 << l; j < step_max; j++)
        {
            tablo->tab[j] = tablo->tab[2*j] + tablo->tab[2*j + 1];
        }
    }
    // descendant step
    tablo->tab[1] = 0;
    for(int l = 0; l<=nbSteps; l++)
    {
        const int step_max = 1 << (l + 1);
#pragma omp parallel for
        for(int j = 1 << l; j < step_max; j++)
        {
            tablo->tab[2*j + 1] = tablo->tab[2*j] + tablo->tab[j];
            tablo->tab[2*j] = tablo->tab[j];
        }
    }
#pragma omp parallel for
    for(int i=0; i<tablo->size / 2; i++) // Sum input and output
    {
        tablo->tab[i + (tablo->size / 2)] += old_tab[i];
    }
    free(old_tab);
}

void parallel_suffix_sum(struct tablo *tablo)
{
    int *old_tab = (int*) malloc((tablo->size / 2) * sizeof(int));
#pragma omp parallel for
    for(int i=0; i<tablo->size / 2; i++) // Copy input in a way to sum it to the output
    {
        old_tab[i] = tablo->tab[i + (tablo->size / 2)];
    }
    int nbSteps = (int) log2(tablo->size / 2) - 1;
    // ascendant step
    for(int l = nbSteps; l>=0; l--)
    {
        const int step_max = 1 << (l + 1);
#pragma omp parallel for
        for(int j = 1 << l; j < step_max; j++)
        {
            tablo->tab[j] = tablo->tab[2*j] + tablo->tab[2*j + 1];
        }
    }
    // descendant step
    tablo->tab[1] = 0;
    for(int l = 0; l<=nbSteps; l++)
    {
        const int step_max = 1 << (l + 1);
#pragma omp parallel for
        for(int j = 1 << l; j < step_max; j++)
        {
            tablo->tab[2*j] = tablo->tab[2*j + 1] + tablo->tab[j];
            tablo->tab[2*j + 1] = tablo->tab[j];
        }
    }
#pragma omp parallel for
    for(int i=0; i<tablo->size / 2; i++) // Sum input and output
    {
        tablo->tab[i + (tablo->size / 2)] += old_tab[i];
    }
    free(old_tab);
}