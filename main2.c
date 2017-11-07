#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))

#define BLOCK_HIGH(id, p, n) \
    (BLOCK_LOW((id) + 1, p, n) - 1)

#define BLOCK_SIZE(id, p, n) \
        (BLOCK_LOW( (id)+1, p, n) - BLOCK_LOW( (id), p, n  ) )

#define BLOCK_OWNER(index, p, n) \
        ( ( ((p)*(index)+1)-1 ) / (n)

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char *argv[])
{
    int id, p, proc0_size, global_count;
    double elapsed_time;
    long long i, n, sqrtn, sqrtn2, low_value, high_value, size, prime, first, count, index;
    char *marked; char * lowprime;

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
    n = atoi(argv[1]);
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
    do
    {
        if (prime * prime > low_value)
            first = (prime * prime - low_value) / 2;
        else
        {
            if (!(low_value % prime))
                first = 0;
            else
                first = (prime - low_value % prime) / 2;
        }
        for (i = first; i < size; i += prime)
            marked[i] = 1;
        if (id == 0)
        {
            while (marked[++index]);
            prime = index*2 + 3;
        } else {
            for (i = prime / 2; i < sqrtn2; i += prime)
                lowprime[i] = 1;
            while (lowprime[++index]);
            prime = index*2 + 3;
        }
    } while (prime <= sqrtn);

    count = 0;
    for (i = 0; i < size; i++)
        if (!marked[i])
            count++;
    MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM,
            0, MPI_COMM_WORLD);
    elapsed_time += MPI_Wtime();
    if (!id)
    {
        printf("%d primes are less than or equal to %d\n",
            global_count+1, n);
        printf("Total elapsed time: %10.6f\n", elapsed_time);
    }
    MPI_Finalize();
    return 0;
}