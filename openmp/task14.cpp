#include <stdio.h>
#include <omp.h>
#include <iostream>

int square_parallel(int n, int nthreads) 
{
    int sum = 0;

    int end = 2 * n - 1;

    #pragma omp parallel for reduction(+ : sum) num_threads(nthreads)
    for (int i = 1; i <= end; i += 2) {
        sum += i;
    }

    return sum;
}

int square_parallel2(int n, int nthreads) 
{
    int sum = 0;

    int end = 2 * n - 1;

    #pragma omp parallel for reduction(+ : sum) num_threads(nthreads)
    for (int i = 1; i <= n; i += 1) {
        sum += i;
    }

    sum = sum + sum - n;

    return sum;
}

int square_serial(int n)
{
    int sum = 0;

    int end = 2 * n - 1;
    for (int i = 1; i <= end; i += 2) {
        sum += i;
    }

    return sum;
}

int main(int argc, char const *argv[])
{
    int N = 210;
    int N_THREADS = 12;

    double t = omp_get_wtime();
    int serial = square_serial(N);
    t = omp_get_wtime() - t;
    printf("Serial result = %d, time = %.7f sec\n", serial, t);

    t = omp_get_wtime();
    int parallel = square_parallel(N, N_THREADS);
    t = omp_get_wtime() - t;
    printf("Parallel result = %d, time = %.7f sec\n", parallel, t);

    t = omp_get_wtime();
    int parallel2 = square_parallel(N, N_THREADS);
    t = omp_get_wtime() - t;
    printf("Parallel result 2 = %d, time = %.7f sec\n", parallel2, t);
}
