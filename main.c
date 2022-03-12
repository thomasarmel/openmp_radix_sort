#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "config.h"

void permute(int *inputArray, int *indexesArray, int *outputArray, int arraySize);
void split(int *inputArray, int *flags, int *outputArray, int arraySize);
void prefix(int *flags, int *outputArray, int arraySize);
void suffix(int *flags, int *outputArray, int arraySize);
void revert(int *inputArray, int *outputArray, int arraySize);

int main(int argc, char *argv[])
{
    Config config = initConfig(argc, argv);
    int size = 10;
    int N = 1021;
    int A[] = {5, 1021, 2, 9, 0, 23, 9, 512, 511, 8};
    int nbBits = log2(N) + 1;
    printf("%d\n", nbBits);
    for(int i = 0; i < nbBits; i++)
    {
        int *bitsForI = (int*)malloc(sizeof(int) * size);
        for(int j = 0; j < size; j++)
        {
            bitsForI[j] = (A[j] >> i) & 1;
        }
        int *tmpArray = (int*)malloc(sizeof(int) * size);
        split(A, bitsForI, tmpArray, size);
        for (int j = 0; j < size; ++j)
        {
            A[j] = tmpArray[j];
        }
        free(bitsForI);
        free(tmpArray);
    }
    for(int i = 0; i < size; i++)
    {
        printf("%d ", A[i]);
    }
    cleanConfig(&config);
    return 0;
}

void permute(int *inputArray, int *indexesArray, int *outputArray, int arraySize)
{
    for(int i = 0; i < arraySize; i++)
    {
        outputArray[indexesArray[i]] = inputArray[i];
    }
}

void split(int *inputArray, int *flags, int *outputArray, int arraySize)
{
    int *indexes = (int*)malloc(sizeof(int) * arraySize);
    int *ldown = (int*)malloc(sizeof(int) * arraySize);
    int *lup = (int*)malloc(sizeof(int) * arraySize);
    int *reversedFlags = (int*)malloc(sizeof(int) * arraySize);
    revert(flags, reversedFlags, arraySize);
    prefix(reversedFlags, ldown, arraySize);
    suffix(flags, lup, arraySize);
    for(int i = 0; i < arraySize; i++)
    {
        lup[i] = (arraySize + 1) - lup[i];
    }
    free(reversedFlags);
    for(int i = 0; i < arraySize; i++)
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
    permute(inputArray, indexes, outputArray, arraySize);
    free(indexes);
    free(ldown);
    free(lup);
}

void prefix(int *flags, int *outputArray, int arraySize)
{
    outputArray[0] = flags[0];
    for(int i = 1; i < arraySize; i++)
    {
        outputArray[i] = outputArray[i - 1] + flags[i];
    }
}

void suffix(int *flags, int *outputArray, int arraySize)
{
    outputArray[arraySize - 1] = flags[arraySize - 1];
    for(int i = arraySize - 2; i >= 0; i--)
    {
        outputArray[i] = outputArray[i + 1] + flags[i];
    }
}

void revert(int *inputArray, int *outputArray, int arraySize)
{
    for(int i = 0; i < arraySize; i++)
    {
        outputArray[i] = !inputArray[i];
    }
}