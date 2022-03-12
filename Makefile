CC = gcc
CFLAGS = -fopenmp -Wall -O3
CLIBS = -lm

radix_sort: main.o config.o parallel_prefix_suffix.o radix_sort.o
	$(CC) -o radix_sort main.o config.o parallel_prefix_suffix.o radix_sort.o $(CFLAGS) $(CLIBS)

%.o : %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f radix_sort *.o