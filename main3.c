#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))

#define BLOCK_HIGH(id, p, n) \
    (BLOCK_LOW((id) + 1, p, n) - 1)

#define BLOCK_SIZE(id, p, n) \
    (BLOCK_LOW((id) + 1, p, n) - BLOCK_LOW((id), p, n))

#define BLOCK_OWNER(index, p, n) \
        ( ( ((p)*(index)+1)-1 ) / (n)

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char *argv[])
{
    int id, p;
    double elapsed_time;
    long long i, n, global_count, proc0_size, sqrtn, sqrtn2, low_value,
        high_value, size, prime, first, count, index, block_size, primes_count;
    char *marked;
    char *lowprime;
    long long *primes;

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
    low_value = 3 + BLOCK_LOW(id, p, n - 1);
    high_value = 3 + BLOCK_HIGH(id, p, n - 1);
    size = BLOCK_SIZE(id, p, n - 1);
    proc0_size = (n - 1) / p;
    sqrtn = (long long)sqrt((double)n);
    if ((3 + proc0_size) < sqrtn)
    {
        if (!id)
            printf("Too many processes\n");
        MPI_Finalize();
        exit(1);
    }
    size = size / 2;

    marked = (char *)malloc(size);
    if (marked == NULL)
    {
        printf("Cannot allocate enough memory\n");
        MPI_Finalize();
        exit(1);
    }

    for (i = 0; i < size; i++)
        marked[i] = 0;
    // if (!id)
    index = 0;
    prime = 3;
    sqrtn2 = sqrtn / 2;
    lowprime = (char *)malloc(sqrtn2);
    if (lowprime == NULL)
    {
        printf("Cannot allocate enough memory\n");
        MPI_Finalize();
        exit(1);
    }
    primes = (long long *)malloc(sqrt2 * sizeof(long long));
    primes_count = -1;
    do
    {
        for (i = prime / 2; i < sqrtn2; i += prime)
            lowprime[i] = 1;
        while (lowprime[++index])
            ;
        prime = index * 2 + 3;
        primes[++primes_count] = prime;
    } while (prime <= sqrtn);

    block_size = 50;
    int k;
    long long t;
    for (t = 0; t < size - block_size; t += block_size)
    {
        for (k = 0; k < primes_count && primes[k] < block_size / 2; ++k)
        {
            prime = primkes[k];
            if (prime * prime > low_value + t * 2)
                first = (prime * prime - low_value - t * 2) / 2;
            else
            {
                if (!((low_value + t * 2) % prime))
                    first = t;
                else
                    first = t + (prime - (low_value + t * 2) % e) / 2;
            }
            for (i = first; i < t + block_size; i += prime)
                marked[i] = 1;
        }
    }

    block_size = size - t;
    for (k = 0; k < primes_count && primes[k] < block_size / 2; ++k)
    {
        prime = primkes[k];
        if (prime * prime > low_value + t * 2)
            first = (prime * prime - low_value - t * 2) / 2;
        else
        {
            if (!((low_value + t * 2) % prime))
                first = t;
            else
                first = t + (prime - (low_value + t * 2) % e) / 2;
        }
        for (i = first; i < t + block_size; i += prime)
            marked[i] = 1;
    }

    for (; k < primes_count; ++k)
    {
        prime = primkes[k];
        if (prime * prime > low_value + t * 2)
            first = (prime * prime - low_value - t * 2) / 2;
        else
        {
            if (!((low_value + t * 2) % prime))
                first = t;
            else
                first = t + (prime - (low_value + t * 2) % e) / 2;
        }
        for (i = first; i < t + block_size; i += prime)
            marked[i] = 1;
    }

    count = 0;
    for (i = 0; i < size; i++)
        if (!marked[i])
            count++;
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
