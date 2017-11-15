#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))

#define BLOCK_HIGH(id, p, n) \
    (BLOCK_LOW((id) + 1, p, n) - 1)

#define BLOCK_SIZE(id, p, n) \
    (BLOCK_LOW((id) + 1, p, n) - BLOCK_LOW((id), p, n))

#define BLOCK_OWNER(index, p, n) \
        ( ( ((p)*(index)+1)-1 ) / (n)

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define setBit(Array, offset) Array[(offset) >> 5] |= (1 << ((offset)&0x1F))
#define getBit(Array, offset) ((Array[(offset) >> 5] & (1 << ((offset)&0x1F))) != 0)
#define ISODD(x) (((x) & 1) == 1)

uint64_t normalCalPrimes(uint64_t primes[], const uint64_t n)
{
	uint64_t prime, i, j, primes_count;
    uint32_t *lowprime;
    lowprime = malloc(sizeof(uint32_t) * (1 + (n >> 6)));
    if (lowprime == NULL)
    {
        fprintf(stderr,"Cannot allocate enough memory\n");
        MPI_Finalize();
        exit(1);
    }
	primes_count = 0;
	for (i = 0; i < (n >> 6); ++i)
		lowprime[i] = 0;
	primes[0] = 2;
	for (i = 0; i < n / 2; ++i)
	{
		if (!getBit(lowprime, i))
		{
			prime = i * 2 + 3;
			primes[++primes_count] = prime;
			for (j = (prime * prime - 3) / 2; j <= n / 2; j += prime)
				setBit(lowprime, j);
		}
	}
	free(lowprime);
	return primes_count + 1;
}


uint64_t partitionSievePrimes(const uint64_t start, const uint64_t size, uint32_t mark[], const uint64_t lowprimes[], const uint64_t primes_count)
{
    uint64_t i, j, prime, first, firstnum, count;
    count = 0;
	firstnum = start * 2 + 3;
	for (i = 0; i <= size >> 5; ++i) mark[i] = 0;
	for (i = 1; (i < primes_count); ++i)
	{
		prime = lowprimes[i];
		if ( (prime * prime - 3) / 2 > start) {
			first = (prime * prime - 3) / 2 - start;
			if (first >= size) break;
		} else {
			if (firstnum % prime == 0) 
				first = 0;
			else {
				if (ISODD(prime - (firstnum % prime)))
					first = (prime*2 - (firstnum % prime)) / 2;
				else
					first = (prime - (firstnum % prime)) / 2;
			}
		}
		for (j = first; j < size; j += prime)
			setBit(mark, j);
	}
	for (i = 0; i < size; ++i)
		if (!getBit(mark, i)) ++count;
	return count;
}



int main(int argc, char *argv[])
{
    int id, p;
    double elapsed_time;
    long long i, n, global_count, proc0_size, sqrtn, low_value,
        high_value, size, size1, prime, first, count, index, block_size, primes_count;
    uint32_t *marked;
    uint64_t *primes;

    MPI_Init(&argc, &argv);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    if (argc != 2)
    {
        if (!id)
            printf("Command line: %s <m>\n", argv[0]);
        MPI_Finalize();
        exit(1);
    }
    n = atoll(argv[1]);
    low_value = BLOCK_LOW(id, p, (n - 1)/2);
    high_value = BLOCK_HIGH(id, p, (n - 1)/2);
    size = BLOCK_SIZE(id, p, (n - 1)/2);
    sqrtn = (long long)sqrt((double)n);

    marked = malloc(sizeof(uint32_t) * (1 + (size >> 5)));
    if (marked == NULL)
    {
        printf("Cannot allocate enough memory\n");
        MPI_Finalize();
        exit(1);
    }

    primes = malloc((sqrtn >> 2) * sizeof(uint64_t));
    primes_count = normalCalPrimes(primes, sqrtn);

    block_size = 2000000 * 8;
    size1 = size / block_size * block_size;
    count = 0;
    for (i = 0; i < size1; i+=block_size) {
        count += partitionSievePrimes(low_value+i, block_size, marked, primes, primes_count);
    }
    if (size1 != size)
        count += partitionSievePrimes(low_value+i, size - size1, marked, primes, primes_count);
   
    MPI_Reduce(&count, &global_count, 1, MPI_LONG_LONG, MPI_SUM,
               0, MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();
    if (!id)
    {
        printf("%lld primes are less than or equal to %lld\n",
               global_count + 1, n);
        printf("Total elapsed time: %10.6f\n", elapsed_time);
    }
    MPI_Finalize();
    return 0;
}
