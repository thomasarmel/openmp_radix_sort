# OpenMP Radix sort

Sort positive integers in ascending order using radix sort.

Parallelization is achieved by using OpenMP.

### Command line

    $ ./radixsort <max value> <array size> [<input file>] <output file> [number of threads]

### Example

Consider the following **input.txt** file:

`5 1021 2 9 0 23 9 512 511 8`

Let's run the program with the following command:

    $ ./radixsort 1021 10 input.txt output.txt 8

Then the **output.txt** file will be:

```
N=1021 Size=10 Threads=8 Time_in_microsec=7
5 1021 2 9 0 23 9 512 511 8
0 2 5 8 9 9 23 511 512 1021
```

### Use special parallel prefix / suffix sum

If you want to compile the program with the special parallel prefix / suffix sum support, build with the following command:

    $ make parallel_prefix_suffix

You can also add **-DUSE_PAR_PRE_SUF** to the compiler options.