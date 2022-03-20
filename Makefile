CC = gcc
CFLAGS = -fopenmp -Wall -O3 -march=native
CLIBS = -lm

all: main.o config.o radix_sort.o
	$(CC) -o radix_sort main.o config.o radix_sort.o $(CFLAGS) $(CLIBS)

parallel_prefix_suffix: main.o config.o parallel_prefix_suffix.o radix_sort_par_pre_suf.o
	$(CC) -o radix_sort main.o config.o parallel_prefix_suffix.o radix_sort_par_pre_suf.o $(CFLAGS) $(CLIBS)

radix_sort_par_pre_suf.o: radix_sort.c
	$(CC) -c radix_sort.c -o radix_sort_par_pre_suf.o -DUSE_PAR_PRE_SUF $(CFLAGS)

%.o : %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f radix_sort *.o